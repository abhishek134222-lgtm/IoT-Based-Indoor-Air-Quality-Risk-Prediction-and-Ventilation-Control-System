# Project Structure

## 1. Repository Overview

The repository is organized to keep the Arduino source code and project documentation separate.

```text
iot-air-quality-ventilation-system/
│
├── src/
│   └── air_quality_ventilation/
│       └── air_quality_ventilation.ino
│
├── docs/
│   ├── project-overview.md
│   ├── working-principle.md
│   ├── components.md
│   ├── system-architecture.md
│   ├── system-flowchart.md
│   ├── objectives.md
│   ├── test-cases.md
│   └── project-structure.md
│
├── README.md
├── LICENSE
└── .gitignore
```

## 2. Folder Description

### `src/`

Contains the source code of the project.

- `air_quality_ventilation/`: Contains the Arduino application.
- `air_quality_ventilation.ino`: Main Arduino sketch responsible for sensor monitoring, risk detection, alerts, Bluetooth communication, and ventilation control.

### `docs/`

Contains the technical documentation.

| File | Description |
|---|---|
| `project-overview.md` | Introduction, problem statement, scope, and expected outcome |
| `working-principle.md` | Explains how the system operates |
| `components.md` | Lists hardware and software components |
| `system-architecture.md` | Describes the system architecture |
| `system-flowchart.md` | Shows the system operation flow |
| `objectives.md` | Describes project objectives and future scope |
| `test-cases.md` | Documents planned system tests |
| `project-structure.md` | Explains the repository organization |

## 3. Root Files

- `README.md`: Main project introduction and setup guide.
- `LICENSE`: MIT License information.
- `.gitignore`: Specifies temporary and unnecessary files that Git should ignore.

## 4. Development Note

The repository structure may be extended as the project develops. Additional source files, circuit diagrams, photographs, and test reports can be added when they are available.