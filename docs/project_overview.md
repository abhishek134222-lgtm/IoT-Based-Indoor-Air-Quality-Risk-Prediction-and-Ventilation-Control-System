# Project Overview

## Project Title

IOT - Based Indoor Air-Quality Risk Prediction and ventilation Control System

## Introduction

Indoor environmental conditions can affect comfort and safety. This project presents an Arduino Uno-based system that monitors selected environmental parameters and responds to potential hazards through automated ventilation and alert mechanisms.

The system combines sensors, embedded control logic, a relay-controlled fan, audio alerts, visual indicators, and Bluetooth communication.

## Problem Statement

Indoor environments may experience smoke, elevated temperatures, or flame-related hazards. A monitoring system that detects these conditions and responds automatically can help provide timely alerts and activate ventilation when required.

## Objectives

- Monitor gas and smoke levels using an MQ-2 sensor.
- Measure temperature and humidity using a DHT11 sensor.
- Detect flame conditions using a flame sensor.
- Activate a ventilation fan when configured alarm conditions occur.
- Provide audio and visual warnings.
- Support manual and automatic fan operation.
- Allow users to monitor system status through Serial Monitor and Bluetooth.
- Store calibrated gas thresholds in EEPROM.

## Project Scope

The current implementation focuses on sensor-based monitoring, threshold-based risk detection, ventilation control, and local Bluetooth communication.

Future development may include additional air-quality sensors, cloud-based monitoring, historical data analysis, and machine-learning-based risk prediction.

## Expected Outcome

The expected outcome is a working prototype capable of monitoring selected environmental conditions, activating the ventilation fan according to its control logic, and notifying users through buzzers and LEDs.

## Limitations

- Gas detection currently uses MQ-2 sensor readings and configurable thresholds.
- Temperature and humidity measurements depend on the DHT11 sensor.
- The system does not currently contain a trained machine-learning model.
- Bluetooth communication requires a compatible nearby device.
- Sensor readings and threshold values are not a substitute for certified safety equipment.

## Conclusion

This project demonstrates the integration of sensors, Arduino programming, embedded control, Bluetooth communication, and automated ventilation in an IoT-oriented environmental monitoring prototype.