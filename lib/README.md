# Private Project Libraries

To keep the project modular and scalable, the logic has been separated into two distinct libraries.

### 1. `hardware` (The "Hands")
Handles all direct interaction with the physical components.
- **Components**: 16x2 LCD, MG90S Servo, HC-SR04 Ultrasonic sensors.
- **Functions**: `readDistance()`, `setGate()`, `updateLCD()`.
- **Goal**: Provides a clean API so the main logic doesn't have to handle low-level pin toggling.

### 2. `traffic_police` (The "Brain")
Manages the decision-making and state of the system.
- **Responsibilities**: 
  - Non-blocking state machine (Red -> Yellow -> Green).
  - Hardware Interrupt (ISR) for the Emergency Priority button.
  - Slot availability logic.
- **Goal**: Operates independently of specific hardware, making it easy to test the logic.