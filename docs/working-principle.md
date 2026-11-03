# Working Principle

## 1. System Initialization

When the Arduino Uno is powered ON:

- Serial communication starts at 9600 baud.
- Bluetooth communication through HC-05 starts at 9600 baud.
- The DHT11 sensor is initialized.
- Previously calibrated gas thresholds are loaded from EEPROM.
- The MQ-2 sensor begins a 20-second warm-up period.
- The system then displays the main command menu.

## 2. Sensor Monitoring

The system uses three primary sensors:

### MQ-2 Gas Sensor
- Reads gas and smoke levels through analog pin A0.
- Compares the readings against configurable ON and OFF thresholds.
- Activates the smoke alarm when the reading reaches the ON threshold.
- Clears the smoke alarm when the reading falls to or below the OFF threshold.

### DHT11 Temperature and Humidity Sensor
- Connected to digital pin D2.
- Measures temperature and relative humidity.
- Readings are updated approximately every 2 seconds.
- High temperature activates ventilation at 35°C.
- The temperature condition clears at or below 33°C.

### Flame Sensor
- Connected to digital pin D3.
- Detects flame using its digital output.
- Requires detection to remain active for 200 milliseconds before confirming a flame condition.
- The flame alarm remains active until manually reset after the flame is no longer detected.

## 3. Risk Detection and Alarm Logic

The controller evaluates three conditions:

| Condition | System Response |
|---|---|
| Smoke detected | Fan ON and smoke buzzer double-beeps for 5 seconds |
| Flame detected | Fan ON and continuous flame buzzer pattern |
| Temperature reaches 35°C | Fan ON until temperature falls to 33°C or below |

The smoke and flame alarm conditions are handled independently.

## 4. Automatic Ventilation Control

A relay module controls the 12V DC ventilation fan.

The fan turns ON when any of the following conditions is true:

- Flame alarm is active.
- Smoke alarm is active.
- High-temperature condition is active.
- User selects manual fan ON.

The fan turns OFF when no alarm is active and the selected fan mode does not request manual operation.

Safety-priority logic prevents manual fan OFF commands from overriding active alarm conditions.

## 5. Audio and Visual Alerts

### Buzzers
Two separate buzzers are used:

- Buzzer 1: Produces a continuous repeating beep pattern for flame detection.
- Buzzer 2: Produces a double-beep pattern for smoke detection.

Users can mute and unmute the buzzers through commands. A newly detected flame automatically unmutes the buzzer.

### LEDs
Two LEDs indicate the system condition:

- Green LED: Indicates normal conditions.
- Red LED: Indicates an alert condition.
- Red LED blinks when a flame alarm is active.

## 6. Bluetooth Communication

The HC-05 Bluetooth module provides wireless communication with the Arduino.

- Arduino software serial uses digital pins D9 and D10.
- Communication speed is 9600 baud.
- Users can send commands to check status, control the fan, manage alarms, and calibrate the MQ-2 sensor.

## 7. Gas Sensor Calibration

The system supports calibration in clean air.

1. The user sends the `calibrate` command.
2. The Arduino collects 50 MQ-2 readings over approximately 5 seconds.
3. The average reading is calculated as the baseline.
4. The OFF threshold is calculated from the baseline with a minimum value of 150.
5. The ON threshold is set 200 points above the OFF threshold.
6. Both thresholds are saved in EEPROM.

The `default` command restores the default thresholds of 350 for ON and 150 for OFF.

## 8. Live Monitoring

When live mode is enabled, the system sends the following information approximately once per second:

- Gas reading.
- Temperature.
- Flame detection status.
- Fan status.

The information is sent through both Serial Monitor and Bluetooth.

## 9. Overall System Flow

```text
       SYSTEM START
             |
             v
      Initialize Arduino
             |
             v
       Load Calibration
             |
             v
       MQ-2 Warm-up
             |
             v
       Read Sensors
             |
             v
    Evaluate Risk Conditions
             |
       +-----+------+
       |            |
       v            v
   Risk Found    Normal
       |            |
       v            v
   Fan ON       Check Fan Mode
       |            |
       v            v
  Audio/Visual   Fan Control
     Alerts          |
       |             |
       +------+------+
              |
              v
       Bluetooth/Serial
         Communication
              |
              v
       Repeat Monitoring
```

## Conclusion

The system continuously monitors selected environmental conditions and applies predefined control logic to activate ventilation and alert mechanisms. It combines sensor monitoring, embedded programming, Bluetooth communication, and automatic control in one Arduino-based prototype.