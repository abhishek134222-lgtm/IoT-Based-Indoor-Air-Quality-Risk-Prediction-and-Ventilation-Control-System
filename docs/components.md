# Hardware and Software Components

## 1. Hardware Components

| Component | Arduino Pin | Purpose |
|---|---|---|
| Arduino Uno | — | Main controller |
| MQ-2 Gas Sensor | A0 | Detects smoke and gas levels |
| DHT11 Sensor | D2 | Measures temperature and humidity |
| Flame Sensor | D3 | Detects flame |
| Buzzer 1 | D4 | Flame alert |
| Green LED | D5 | Normal status indication |
| Red LED | D6 | Warning indication |
| Relay Module | D7 | Controls the ventilation fan (active LOW) |
| Buzzer 2 | D8 | Smoke alert |
| HC-05 Bluetooth Module | D9 (RX), D10 (TX) | Wireless serial communication |
| 12V DC Fan | Relay output | Provides ventilation |

## 2. HC-05 Bluetooth Connections

| HC-05 Pin | Arduino Uno Connection |
|---|---|
| TXD | D9 (Arduino RX) |
| RXD | D10 (Arduino TX) through a voltage divider |
| VCC | 5V |
| GND | GND |

**Communication baud rate:** 9600.

## 3. Software Components

| Technology / Library | Purpose |
|---|---|
| Arduino IDE | Writing and uploading the sketch |
| Arduino C/C++ | Programming language |
| DHT Library | Reading temperature and humidity |
| SoftwareSerial | Bluetooth serial communication |
| EEPROM | Saving calibration values |
| Serial Monitor | Monitoring readings and sending commands |

## 4. Communication

The system supports:
- Serial communication through the Arduino Serial Monitor.
- Bluetooth communication through the HC-05 module.
- 9600 baud communication speed.

**Note:** Internet and cloud connectivity are not implemented in the current version.

## 5. Safety Notes

- Use an appropriate power supply for the 12V fan.
- Never power the fan directly from an Arduino GPIO pin.
- Use a suitable relay module to control the fan.
- Use a voltage divider between Arduino D10 and HC-05 RXD.
- Check wiring before powering the circuit.

## 6. Future Improvements

An MQ-135 sensor may be considered for future air-quality monitoring. It is not included in the current implementation.