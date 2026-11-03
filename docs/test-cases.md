# System Test Cases

## 1. Introduction

This document describes test cases for checking the main functions of the IoT-Based Indoor Air-Quality Risk Prediction and Ventilation Control System.

Testing should be performed with the Arduino Uno, sensors, relay, fan, LEDs, buzzers, and HC-05 module connected correctly.

## 2. Test Cases

| Test ID | Test Scenario | Expected Result |
|---|---|---|
| TC-01 | Power on the Arduino | System initializes its components |
| TC-02 | Start the system with the MQ-2 sensor | Sensor warm-up period runs |
| TC-03 | Gas reading crosses the ON threshold | Smoke alert activates and fan turns ON |
| TC-04 | Gas reading falls below the OFF threshold | Smoke alert clears |
| TC-05 | Temperature reaches 35°C or above | Temperature-based fan control turns the fan ON |
| TC-06 | Temperature falls to 33°C or below | Temperature-based fan control allows the fan to turn OFF if no other condition requires it |
| TC-07 | Flame sensor detects a confirmed flame | Flame alarm activates and fan remains ON |
| TC-08 | Send the `on` command | Fan switches to manual ON mode |
| TC-09 | Send the `off` command | Fan switches to manual OFF mode unless an active alarm overrides it |
| TC-10 | Send the `auto` command | Automatic fan-control mode is enabled |
| TC-11 | Send the `mute` command | Audible alerts are muted |
| TC-12 | Send the `unmute` command | Audible alerts are enabled again |
| TC-13 | Send the `status` command | Current system status is displayed |
| TC-14 | Send the `live` command | Live monitoring output is enabled |
| TC-15 | Send the `reset` command | Resettable flame alarm condition is cleared |
| TC-16 | Send the `calibrate` command | Gas sensor calibration process starts |
| TC-17 | Restart the Arduino after calibration | Stored calibration values are retrieved from EEPROM |
| TC-18 | Send commands through Bluetooth | Supported commands are processed when Bluetooth communication is connected |

## 3. Testing Procedure

1. Check all sensor and component connections.
2. Power on the Arduino and allow the MQ-2 warm-up period to complete.
3. Observe sensor readings through the Serial Monitor.
4. Test gas and temperature conditions carefully.
5. Verify flame detection and alarm behaviour using a safe test setup.
6. Test manual and automatic fan-control commands.
7. Test mute, unmute, reset, and status commands.
8. Check Bluetooth communication if the HC-05 is connected.
9. Record the actual result of each test.

## 4. Test Result Record

Use the following status values when recording test results:

- **PASS:** Actual result matches the expected result.
- **FAIL:** Actual result does not match the expected result.
- **NOT TESTED:** Test has not yet been performed.

## 5. Safety Precautions

- Do not create an actual fire to test the flame sensor.
- Do not expose people to smoke or harmful gases during testing.
- Use safe, controlled methods to test sensors.
- Ensure the fan and relay use an appropriate power supply.
- Disconnect power before changing circuit connections.

## 6. Testing Limitation

These are planned test cases. Their actual PASS or FAIL status must be recorded only after performing the tests on the hardware. The current project uses threshold-based detection rather than a trained machine-learning model.