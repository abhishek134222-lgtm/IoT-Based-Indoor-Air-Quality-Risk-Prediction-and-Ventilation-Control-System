# System Flowchart

## 1. System Operation Flow

The following flowchart represents the working sequence of the Indoor Air-Quality Risk Detection and Ventilation Control System.

```mermaid
flowchart TD
    A([Start]) --> B[Initialize Arduino and Sensors]
    B --> C[Wait for MQ-2 Warm-up]
    C --> D[Read Sensor Data]
    D --> E[Check Gas / Smoke Level]
    E --> F[Check Temperature]
    F --> G[Check Flame Sensor]
    G --> H{Flame Detected?}

    H -- Yes --> I[Activate Flame Alarm]
    I --> J[Keep Fan ON Until Reset]

    H -- No --> K{Gas Level Above ON Threshold?}
    K -- Yes --> L[Activate Smoke Alarm and Fan]
    K -- No --> M{Gas Level Below OFF Threshold?}
    M -- Yes --> N[Clear Smoke Alarm]
    M -- No --> O[Maintain Previous Smoke State]

    L --> P[Apply Fan Control Logic]
    N --> P
    O --> P
    J --> Q[Update LEDs and Buzzer]
    P --> Q

    Q --> R[Send Status Through Serial / Bluetooth]
    R --> S{Reset or Control Command?}
    S -- Yes --> T[Process User Command]
    S -- No --> D
    T --> D
```

## 2. Explanation

1. The Arduino initializes the sensors and connected components.
2. The MQ-2 sensor is given time to warm up.
3. The system continuously reads sensor values.
4. Gas, temperature, and flame conditions are evaluated.
5. If a flame is detected, the flame alarm is activated and the fan remains ON until reset.
6. If smoke or gas crosses the ON threshold, the smoke alarm and fan are activated.
7. When the gas level falls below the OFF threshold, the smoke alarm is cleared.
8. The system updates the LEDs, buzzers, and fan according to its control logic.
9. Current status is made available through Serial and Bluetooth communication.
10. User commands are processed before the next monitoring cycle.

## 3. Important Note

This flowchart describes the current rule-based implementation. It does not represent a trained machine-learning prediction process. The actual fan operation also depends on the selected operating mode and active alarm conditions.