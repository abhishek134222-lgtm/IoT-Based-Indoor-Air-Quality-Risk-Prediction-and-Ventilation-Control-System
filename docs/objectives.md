# Project Objectives

## 1. Main Objective

To develop an IoT-based indoor air-quality monitoring system that detects potential environmental risks and automatically controls ventilation using an Arduino Uno.

## 2. Specific Objectives

- Monitor indoor gas and smoke levels using an MQ-2 sensor.
- Measure temperature and humidity using a DHT11 sensor.
- Detect possible fire conditions using a flame sensor.
- Identify potential risks using predefined thresholds and control conditions.
- Activate visual and audible alerts when abnormal conditions are detected.
- Automatically control a ventilation fan through a relay module.
- Provide automatic and manual fan-control modes.
- Allow users to monitor system status and send commands through Bluetooth and Serial communication.
- Store gas calibration values in EEPROM so they can be retained after restarting the system.

## 3. Expected Outcome

The expected outcome is a working prototype capable of monitoring selected indoor environmental conditions, providing alerts, and controlling a ventilation fan based on programmed rules.

## 4. Future Scope

- Integrate an MQ-135 sensor for additional air-quality measurements.
- Explore machine-learning techniques for predictive risk analysis.
- Add internet-based monitoring and remote access.
- Develop a dedicated mobile application.
- Improve sensor calibration and detection accuracy.

## 5. Current Implementation Note

The present prototype uses threshold-based detection and predefined control logic. Machine-learning prediction and cloud-based monitoring are future improvements, not currently implemented features.