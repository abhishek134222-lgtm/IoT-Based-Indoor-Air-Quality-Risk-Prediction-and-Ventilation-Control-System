# IOT - Based Indoor Air-Quality Risk Prediction and ventilation Control System

An Arduino Uno-based IoT project designed to monitor indoor environmental conditions, detect potential air-quality risks, and automatically control ventilation.

## Overview

This project uses multiple sensors to monitor indoor environmental conditions and identify potential risks such as smoke, high temperature, and flame detection.

The system processes sensor readings through an Arduino Uno and responds using automatic ventilation, audio alerts, and visual indicators. Users can also monitor system information and control the fan through Bluetooth communication.

## Features

- Real-time gas and smoke monitoring using MQ-2.
- Temperature and humidity monitoring using DHT11.
- Flame detection with alarm latching.
- Automatic ventilation using a relay-controlled 12V fan.
- Separate buzzer alerts for flame and smoke.
- Red and green LED status indicators.
- Wireless communication using HC-05 Bluetooth.
- Manual ON, OFF, and automatic fan modes.
- Gas threshold calibration with EEPROM storage.
- Live sensor monitoring through Serial Monitor and Bluetooth.
- Safety-priority fan control during alarm conditions.

## Hardware Components

 -------------------------------------------------------
|    Component    |               Purpose               |
|-----------------|-------------------------------------|
| Arduino Uno     | Main controller                     |
| MQ-2 Sensor     | Gas and smoke detection             |
| DHT11 Sensor    | Temperature and humidity monitoring |
| Flame Sensor    | Flame detection                     |
| HC-05 Module    | Bluetooth communication             |
| 1-Channel Relay | Fan switching                       |
| 12V DC Fan      | Ventilation                         |
| 2 Buzzers       | Audio alerts                        |
| 2 LEDs          | Visual indicators                   |
 -------------------------------------------------------

## Technologies Used

- Arduino C/C++
- Arduino Uno
- Embedded Systems
- Internet of Things (IoT)
- Bluetooth Communication
- Sensor-Based Automation
- EEPROM Data Storage

## Working Principle

1. Sensors continuously collect environmental readings.
2. Arduino processes the collected sensor data.
3. The MQ-2 sensor detects smoke or gas levels.
4. The DHT11 monitors temperature and humidity.
5. The flame sensor detects potential fire conditions.
6. The controller evaluates predefined thresholds and alarm conditions.
7. The relay activates the ventilation fan when required.
8. Buzzers and LEDs provide audio and visual alerts.
9. Users can monitor readings and control the system through Serial Monitor or Bluetooth.

## Bluetooth Commands

|   Command   |          Function          |
|-------------|----------------------------|
| `status`    | Display system status      |
| `on`        | Turn fan ON                |
| `off`       | Turn fan OFF               |
| `auto`      | Enable automatic mode      |
| `mute`      | Mute buzzers               |
| `unmute`    | Enable buzzers             |
| `reset`     | Reset flame alarm          |
| `live`      | Toggle live sensor data    |
| `calibrate` | Calibrate MQ-2             |
| `default`   | Restore default thresholds |
| `test`      | Test fan and buzzers       |
| `menu`      | Display command menu       |

The additional command `hawa` is supported for turning the fan ON.

## Project Structure

```text
iot-air-quality-ventilation-system/
│
├── src/
│   └── air_quality_ventilation/
│       └── air_quality_ventilation.ino
│
├── README.md
├── LICENSE
└── .gitignore
```

## Software Requirements

- Arduino IDE
- Arduino Uno board support
- DHT sensor library
- SoftwareSerial library
- EEPROM library

## Getting Started

1. Clone this repository.
2. Open the Arduino IDE.
3. Open `src/air_quality_ventilation/air_quality_ventilation.ino`.
4. Install the required DHT library.
5. Connect the hardware according to the circuit documentation.
6. Select Arduino Uno and the correct COM port.
7. Upload the program.
8. Open Serial Monitor at 9600 baud.

## Current Implementation

The current implementation uses sensor readings and predefined thresholds for risk detection and ventilation control.

It does not currently implement a trained machine-learning prediction model.

## Future Improvements

- Integration of an MQ-135 air-quality sensor.
- Advanced indoor air-quality assessment.
- Machine-learning-based risk prediction.
- IoT cloud dashboard and remote monitoring.
- Historical sensor data storage and visualization.
- Mobile application integration.

## Safety Notice

This project is an educational prototype and is not a certified fire alarm, gas detector, or life-safety ventilation system.

Always follow appropriate electrical safety practices when connecting the relay, fan, and power supply.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

## 👥 Project Contributors

|        Name          |     Role     |         GitHub       
|----------------------|--------------|-----------------------------
| **Abhishek**         | Project Lead | ([Abhishek](https://github.com/abhishek134222-lgtm)) 
| **Daman Bassi**      | Contributor  | ([Daman Bassi](https://github.com/Daman072008))         
| **Bipanpreet Singh** | Contributor  | ([Bipanreet Singh](https://github.com/bipanpreets28))       
| **Anuj**             | Contributor  | ([Anuj](https://github.com/techanuj00-sketch))   

