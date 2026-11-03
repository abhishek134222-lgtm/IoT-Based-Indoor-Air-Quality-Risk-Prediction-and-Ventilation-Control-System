# System Architecture

## 1. Overview

The system uses an Arduino Uno as its main controller. It receives information from different sensors, processes the readings, detects possible indoor air-quality risks, and controls alerts and ventilation.

## 2. Main System Components

The system consists of the following sections:

1. **Input Section:** MQ-2 gas sensor, DHT11 temperature and humidity sensor, and flame sensor.
2. **Processing Section:** Arduino Uno processes sensor readings and applies predefined thresholds and control conditions.
3. **Alert Section:** LEDs and two buzzers provide visual and audible warnings.
4. **Ventilation Section:** A relay module controls the 12V DC fan.
5. **Communication Section:** HC-05 Bluetooth module and USB Serial communication allow users to monitor the system and send commands.

## 3. System Workflow

1. The sensors collect environmental readings.
2. The Arduino reads gas levels, temperature, humidity, and flame status.
3. The program checks readings against predefined conditions.
4. If a risk is detected, the appropriate warning is activated.
5. The relay controls the ventilation fan according to the system's control logic.
6. Sensor readings and system status can be monitored through Serial or Bluetooth communication.

## 4. Architecture Diagram

```text
       ENVIRONMENT
            |
            v
   +--------------------+
   |   INPUT SENSORS    |
   |--------------------|
   | MQ-2 Gas Sensor    |
   | DHT11 Sensor       |
   | Flame Sensor       |
   +--------------------+
            |
            v
   +--------------------+
   |    ARDUINO UNO     |
   |--------------------|
   | Read Sensor Data   |
   | Check Thresholds   |
   | Apply Control Logic|
   +--------------------+
       |       |       |
       v       v       v
   +-------+ +------+ +----------------+
   | Alerts| |Relay | | Communication  |
   | LEDs &| |Module| | HC-05 / Serial |
   |Buzzers| +------+ +----------------+
   +-------+    |
                v
          +-----------+
          | 12V Fan   |
          +-----------+
```

## 5. Control Logic

- **Gas/Smoke Detection:** The MQ-2 reading is checked against configurable ON and OFF thresholds.
- **Temperature Control:** The fan turns ON at or above 35°C and turns OFF at or below 33°C, subject to other active conditions.
- **Flame Detection:** A confirmed flame activates the flame alarm and keeps the fan ON until the reset condition is met.
- **Manual Control:** Users can select automatic, manual ON, or manual OFF modes. Active alarms can override manual OFF.
- **Communication:** Users can monitor system information and send supported commands through Serial or Bluetooth.

## 6. Implementation Note

The current implementation uses predefined thresholds and rule-based control logic. It does not currently contain a trained machine-learning prediction model or internet/cloud integration.