/*
  IoT Air Ventilation + Safety System  (FINAL)
  Board : Arduino Uno
  Parts : MQ2, DHT11, Flame sensor, 2 buzzers, 2 LEDs, 1-ch relay (12V fan), HC-05

  Alarm logic:
   - Flame  : fan ON, buzzer 1 beeps continuously until user resets (menu 7)
   - Smoke  : fan ON, buzzer 2 double-beeps 5 sec, fan stays ON until gas <= gasOff
   - High temperature : fan ON until it cools down
   - Fan is ON in every alarm condition (manual OFF cannot override an alarm)
*/

#include <DHT.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>

// ======================= PINS =======================
#define MQ2_PIN      A0
#define DHT_PIN      2
#define FLAME_PIN    3
#define BUZZER1_PIN  4     // flame buzzer
#define GREEN_LED    5
#define RED_LED      6
#define RELAY_PIN    7
#define BUZZER2_PIN  8     // smoke buzzer
#define BT_RX_PIN    9     // HC-05 TXD -> D9
#define BT_TX_PIN    10    // D10 -> 1k/2k divider -> HC-05 RXD

// ======================= CONFIG =======================
#define DHTTYPE          DHT11
#define PASSIVE_BUZZER   false        // set true if your buzzers are passive

const int RELAY_ON        = LOW;      // active LOW relay module
const int RELAY_OFF       = HIGH;
const int FLAME_DETECTED  = LOW;      // flame sensor DO goes LOW on flame

const float TEMP_FAN_ON   = 35.0;     // fan ON above this (deg C)
const float TEMP_FAN_OFF  = 33.0;     // fan OFF below this
const uint8_t SMOKE_BEEP_SECONDS = 5; // double-beep duration
const unsigned long FLAME_CONFIRM_MS = 200;

const char* CUSTOM_FAN_ON_WORD = "hawa";   // extra word for fan ON (lowercase)

// Smoke thresholds (changed by calibration, saved in EEPROM)
int gasOn  = 350;                     // smoke detected at/above this
int gasOff = 150;                     // smoke cleared at/below this

// ======================= OBJECTS =======================
DHT dht(DHT_PIN, DHTTYPE);
SoftwareSerial bt(BT_RX_PIN, BT_TX_PIN);

// Prints to Serial Monitor and Bluetooth together
class Tee : public Print {
public:
  size_t write(uint8_t c) override {
    Serial.write(c);
    bt.write(c);
    return 1;
  }
};
Tee out;

// ======================= BUZZER PATTERNS =======================
struct Note { uint16_t freq; uint16_t ms; };   // freq 0 = silence

const Note PATTERN_FLAME[] = { {2500, 300}, {0, 300} };                         // continuous beep
const Note PATTERN_SMOKE[] = { {2000, 100}, {0, 100}, {2000, 100}, {0, 700} };  // double beep

struct Player {
  const Note* pat;
  uint8_t len;
  uint8_t idx;
  unsigned long t;
  bool active;
  uint8_t pin;
};

Player flameBuzz = { PATTERN_FLAME, 2, 0, 0, false, BUZZER1_PIN };
Player smokeBuzz = { PATTERN_SMOKE, 4, 0, 0, false, BUZZER2_PIN };

void setBuzzer(uint8_t pin, uint16_t freq) {
  if (PASSIVE_BUZZER) {
    if (freq == 0) noTone(pin); else tone(pin, freq);
  } else {
    digitalWrite(pin, freq == 0 ? LOW : HIGH);
  }
}

void runPattern(Player &p, bool enable) {
  if (!enable) {
    if (p.active) { setBuzzer(p.pin, 0); p.active = false; }
    return;
  }
  if (!p.active) {
    p.active = true;
    p.idx = 0;
    p.t = millis();
    setBuzzer(p.pin, p.pat[0].freq);
    return;
  }
  if (millis() - p.t >= p.pat[p.idx].ms) {
    p.idx = (p.idx + 1) % p.len;
    p.t = millis();
    setBuzzer(p.pin, p.pat[p.idx].freq);
  }
}

// ======================= STATE =======================
int   gasValue    = 0;
float temperature = 0;
float humidity    = 0;

bool flameNow     = false;     // flame confirmed right now
bool flameSeen    = false;
unsigned long flameSince = 0;

bool flameLatched = false;     // stays until user resets
bool smokeLatched = false;     // stays until gas <= gasOff
bool tempFan      = false;
bool fanOn        = false;
bool muted        = false;
bool liveMode     = false;
uint8_t fanMode   = 0;         // 0 = auto, 1 = manual ON, 2 = manual OFF

bool smokeBeeping = false;
unsigned long smokeBeepStart = 0;

unsigned long lastDhtRead = 0;
unsigned long lastLive    = 0;
uint8_t dhtFails = 0;
bool dhtFaultShown = false;

// ======================= EEPROM CALIBRATION =======================
struct Cal { uint16_t magic; int16_t gasOn; int16_t gasOff; };
const uint16_t CAL_MAGIC = 0xA5A5;

void loadCal() {
  Cal c;
  EEPROM.get(0, c);
  if (c.magic == CAL_MAGIC && c.gasOff > 0 && c.gasOn > c.gasOff && c.gasOn <= 1000) {
    gasOn  = c.gasOn;
    gasOff = c.gasOff;
  }
}

void saveCal() {
  Cal c = { CAL_MAGIC, (int16_t)gasOn, (int16_t)gasOff };
  EEPROM.put(0, c);
}

void calibrate() {
  out.println(F("Calibrating in clean air (5 sec)... keep smoke away."));
  long sum = 0;
  for (int i = 0; i < 50; i++) { sum += analogRead(MQ2_PIN); delay(100); }
  int base = sum / 50;
  gasOff = max(150, base + 40);
  gasOn  = constrain(gasOff + 200, 0, 1000);
  saveCal();
  out.print(F("Baseline: ")); out.println(base);
  out.print(F("Smoke ON at: ")); out.print(gasOn);
  out.print(F(" | Fan OFF at: ")); out.println(gasOff);
  if (gasOn <= gasOff + 50) out.println(F("Warning: baseline too high. Warm up MQ2 longer and retry."));
}

// ======================= MENU / PRINT =======================
void printMenu() {
  out.println();
  out.println(F("=========== MAIN MENU ==========="));
  out.println(F(" 1  Status"));
  out.println(F(" 2  Fan ON"));
  out.println(F(" 3  Fan OFF"));
  out.println(F(" 4  Fan AUTO"));
  out.println(F(" 5  Mute buzzers"));
  out.println(F(" 6  Unmute buzzers"));
  out.println(F(" 7  Reset flame alarm"));
  out.println(F(" 8  Live data ON/OFF"));
  out.println(F(" 9  Calibrate MQ2 (clean air)"));
  out.println(F("10  Default thresholds"));
  out.println(F("11  Test fan + buzzers"));
  out.println(F(" 0  Show this menu"));
  out.println(F("--------------------------------"));
  out.println(F("Words also work: on, off, auto,"));
  out.println(F("mute, reset, status, hawa..."));
  out.println(F("================================"));
  out.println(F("Send a number:"));
}

void printStatus() {
  out.println(F("--- STATUS ---"));
  out.print(F("Gas         : ")); out.print(gasValue);
  out.print(F("  (ON>=")); out.print(gasOn);
  out.print(F(", OFF<="));  out.print(gasOff); out.println(F(")"));
  out.print(F("Temp / Hum  : ")); out.print(temperature, 1);
  out.print(F(" C / ")); out.print(humidity, 0); out.println(F(" %"));
  out.print(F("Flame now   : ")); out.println(flameNow ? F("YES") : F("NO"));
  out.print(F("Flame alarm : ")); out.println(flameLatched ? F("ACTIVE (send 7 to reset)") : F("OK"));
  out.print(F("Smoke       : ")); out.println(smokeLatched ? F("HIGH") : F("OK"));
  out.print(F("Fan         : ")); out.print(fanOn ? F("ON") : F("OFF"));
  out.print(F(" (mode: "));
  out.print(fanMode == 0 ? F("auto") : (fanMode == 1 ? F("manual ON") : F("manual OFF")));
  out.println(F(")"));
  out.print(F("Buzzers     : ")); out.println(muted ? F("MUTED") : F("ENABLED"));
  out.print(F("Uptime      : ")); out.print(millis() / 1000); out.println(F(" s"));
}

// ======================= COMMANDS =======================
String cmdBuf = "";
unsigned long lastCharTime = 0;

void testOutputs() {
  out.println(F("Test: fan + buzzer 1 + buzzer 2"));
  digitalWrite(RELAY_PIN, RELAY_ON);
  setBuzzer(BUZZER1_PIN, 2000); delay(1000); setBuzzer(BUZZER1_PIN, 0);
  setBuzzer(BUZZER2_PIN, 2000); delay(1000); setBuzzer(BUZZER2_PIN, 0);
  digitalWrite(RELAY_PIN, RELAY_OFF);
  out.println(F("Test done."));
}

// Converts menu numbers to command words
String menuToCommand(const String &cmd) {
  if (cmd == "0")  return "menu";
  if (cmd == "1")  return "status";
  if (cmd == "2")  return "on";
  if (cmd == "3")  return "off";
  if (cmd == "4")  return "auto";
  if (cmd == "5")  return "mute";
  if (cmd == "6")  return "unmute";
  if (cmd == "7")  return "reset";
  if (cmd == "8")  return "live";
  if (cmd == "9")  return "calibrate";
  if (cmd == "10") return "default";
  if (cmd == "11") return "test";
  return cmd;
}

void executeCommand(String cmd) {
  cmd.trim();
  cmd.toLowerCase();
  if (cmd.length() == 0) return;
  cmd = menuToCommand(cmd);

  if (cmd == "r" || cmd == "reset") {
    if (flameNow) {
      out.println(F("Flame still detected! Remove flame first."));
    } else {
      flameLatched = false;
      muted = false;
      out.println(F("Flame alarm reset."));
    }
  }
  else if (cmd == "s" || cmd == "status") {
    printStatus();
  }
  else if (cmd == "m" || cmd == "mute") {
    muted = true;
    out.println(F("Buzzers muted (fan and LEDs still work)."));
  }
  else if (cmd == "u" || cmd == "unmute") {
    muted = false;
    out.println(F("Buzzers unmuted."));
  }
  else if (cmd == "f" || cmd == "on" || cmd == "fan on" || cmd == "fanon" ||
           cmd == "start" || cmd == "run" || cmd == CUSTOM_FAN_ON_WORD) {
    fanMode = 1;
    out.println(F("Fan ON (manual)."));
  }
  else if (cmd == "x" || cmd == "off" || cmd == "fan off" || cmd == "fanoff" || cmd == "stop") {
    fanMode = 2;
    if (flameLatched || smokeLatched || tempFan) out.println(F("Alarm active - fan stays ON until it clears."));
    else out.println(F("Fan OFF (manual)."));
  }
  else if (cmd == "a" || cmd == "auto") {
    fanMode = 0;
    out.println(F("Fan AUTO mode."));
  }
  else if (cmd == "l" || cmd == "live") {
    liveMode = !liveMode;
    out.println(liveMode ? F("Live data ON") : F("Live data OFF"));
  }
  else if (cmd == "c" || cmd == "calibrate") {
    calibrate();
  }
  else if (cmd == "d" || cmd == "default") {
    gasOn = 350; gasOff = 150; saveCal();
    out.println(F("Default thresholds restored (ON 350, OFF 150)."));
  }
  else if (cmd == "t" || cmd == "test") {
    testOutputs();
  }
  else if (cmd == "menu" || cmd == "h" || cmd == "help") {
    printMenu();
  }
  else {
    out.println(F("Unknown option. Send 0 for menu."));
  }
}

void handleCommands() {
  while (Serial.available() || bt.available()) {
    char ch = Serial.available() ? Serial.read() : bt.read();
    if (ch == '\n' || ch == '\r') {
      if (cmdBuf.length()) { executeCommand(cmdBuf); cmdBuf = ""; }
    } else if (cmdBuf.length() < 20) {
      cmdBuf += ch;
      lastCharTime = millis();
    }
  }
  // "No line ending" case: run command after a short pause
  if (cmdBuf.length() && millis() - lastCharTime > 300) {
    executeCommand(cmdBuf);
    cmdBuf = "";
  }
}

// ======================= SENSORS =======================
void readSensors() {
  gasValue = analogRead(MQ2_PIN);

  // Flame must stay detected for FLAME_CONFIRM_MS (avoids false alarms)
  if (digitalRead(FLAME_PIN) == FLAME_DETECTED) {
    if (!flameSeen) { flameSeen = true; flameSince = millis(); }
    flameNow = (millis() - flameSince >= FLAME_CONFIRM_MS);
  } else {
    flameSeen = false;
    flameNow = false;
  }

  // DHT11 can only be read every ~2 seconds
  if (millis() - lastDhtRead >= 2000) {
    lastDhtRead = millis();
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if (!isnan(t) && !isnan(h)) {
      temperature = t;
      humidity = h;
      dhtFails = 0;
      dhtFaultShown = false;
    } else if (dhtFails < 255) {
      dhtFails++;
      if (dhtFails >= 5 && !dhtFaultShown) {
        dhtFaultShown = true;
        out.println(F("Warning: DHT11 not responding. Check wiring (D2, 5V, GND)."));
      }
    }
  }
}

// ======================= LOGIC =======================
void updateAlarms() {
  // Flame (latched until reset)
  if (flameNow && !flameLatched) {
    flameLatched = true;
    muted = false;   // new flame always unmutes the buzzer
    out.println(F("!! FLAME DETECTED - fan ON, buzzer beeping. Send 7 to reset."));
  }

  // Smoke (fan ON at gasOn, OFF at gasOff)
  if (gasValue >= gasOn && !smokeLatched) {
    smokeLatched = true;
    smokeBeeping = true;
    smokeBeepStart = millis();
    out.println(F("!! SMOKE DETECTED - fan ON, double beep started."));
  } else if (gasValue <= gasOff && smokeLatched) {
    smokeLatched = false;
    smokeBeeping = false;
    out.println(F("Smoke cleared."));
  }

  // Temperature (with hysteresis)
  if (temperature >= TEMP_FAN_ON) {
    if (!tempFan) out.println(F("High temperature - fan ON."));
    tempFan = true;
  } else if (temperature <= TEMP_FAN_OFF) {
    tempFan = false;
  }
}

void updateFan() {
  // Alarms always turn the fan ON; manual OFF cannot override them
  fanOn = flameLatched || smokeLatched || tempFan || (fanMode == 1);
  digitalWrite(RELAY_PIN, fanOn ? RELAY_ON : RELAY_OFF);
}

void updateBuzzers() {
  if (smokeBeeping && (millis() - smokeBeepStart >= SMOKE_BEEP_SECONDS * 1000UL)) {
    smokeBeeping = false;   // double beeps finished, fan continues
  }
  runPattern(flameBuzz, flameLatched && !muted);
  // passive buzzers: Uno can play only one tone() at a time, flame has priority
  runPattern(smokeBuzz, smokeBeeping && !muted && !(PASSIVE_BUZZER && flameLatched));
}

void updateLeds() {
  bool alert = flameLatched || smokeLatched || tempFan;
  bool redState = flameLatched ? ((millis() / 200) % 2 == 0) : alert;   // red blinks on flame
  digitalWrite(RED_LED,   redState ? HIGH : LOW);
  digitalWrite(GREEN_LED, alert ? LOW : HIGH);
}

void printLive() {
  if (!liveMode || millis() - lastLive < 1000) return;
  lastLive = millis();
  out.print(F("Gas: "));      out.print(gasValue);
  out.print(F(" | Temp: "));  out.print(temperature, 1);
  out.print(F(" | Flame: ")); out.print(flameNow ? F("YES") : F("NO"));
  out.print(F(" | Fan: "));   out.println(fanOn ? F("ON") : F("OFF"));
}

// Warm-up that still watches for flame and accepts commands
void warmUp(unsigned long ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    readSensors();
    if (flameNow && !flameLatched) {
      flameLatched = true;
      muted = false;
      out.println(F("!! FLAME DETECTED during warm-up. Send 7 to reset."));
    }
    handleCommands();
    updateFan();
    updateBuzzers();
    if (flameLatched) updateLeds();
  }
}

// ======================= SETUP / LOOP =======================
void setup() {
  Serial.begin(9600);
  bt.begin(9600);

  digitalWrite(RELAY_PIN, RELAY_OFF);   // fan OFF at boot
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER1_PIN, OUTPUT);
  pinMode(BUZZER2_PIN, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(FLAME_PIN, INPUT);

  dht.begin();
  loadCal();

  out.println(F("MQ2 warming up (20s)..."));
  digitalWrite(RED_LED, HIGH);
  warmUp(20000);
  digitalWrite(RED_LED, LOW);

  setBuzzer(BUZZER1_PIN, 2000); delay(100); setBuzzer(BUZZER1_PIN, 0);   // ready chirp
  out.println(F("System ready."));
  printMenu();
}

void loop() {
  readSensors();
  handleCommands();
  updateAlarms();
  updateFan();
  updateBuzzers();
  updateLeds();
  printLive();
}