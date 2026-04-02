# Smart Parking and Emergency Priority Traffic System

## Introduction
In modern urban environments, inefficient parking management and rigid traffic control systems contribute significantly to congestion and delays. Drivers often spend excessive time searching for available parking spaces, while emergency vehicles like ambulances face critical delays due to unresponsive traffic signals. 

This project proposes a cohesive solution: a Smart Parking and Emergency Priority Traffic System. By integrating real-time parking availability tracking with an intelligent, priority-aware traffic light and gate control system, this project aims to optimize parking space utilization, reduce traffic congestion, and ensure uninterrupted passage for emergency vehicles.

## Key Features
* **Real-Time Capacity Tracking:** Monitors 6 independent parking slots simultaneously using an optimized shared-trigger ultrasonic array.
* **Dynamic Gate Actuation:** Gate opening speed is mapped to the yellow traffic light phase for realistic, gradual mechanical movement.
* **Smart Traffic State Machine:** Traffic lights automatically lock on Red if the lot reaches maximum capacity, preventing gridlock.
* **Low Capacity Warning:** The green traffic light blinks to visually warn approaching drivers when 1 or fewer slots remain.
* **Instant Emergency Override:** A hardware interrupt allows emergency vehicles to bypass the timer, instantly turning the light green and forcing the gate open for 5 seconds.

## Hardware Components
* **1x Arduino Uno (ATmega328P):** The main processing unit.
* **6x Ultrasonic Sensors (HC-SR04):** To monitor individual parking slots.
* **1x Micro Servo Motor:** Acts as the physical entry gate.
* **1x 16x2 LCD Display:** Provides a real-time user interface.
* **3x LEDs (Red, Yellow, Green):** Simulates the incoming traffic light.
* **1x Push Button:** Triggers the emergency override.

## System Architecture & Logic
The system operates using a non-blocking state machine (utilizing `millis()`) and a hardware interrupt to coordinate three main subsystems:

1. **Automated Parking Management:** A single master trigger pin pulses 6 distinct ultrasonic echo pins. The distance returned determines if a car is present. The system calculates the total available slots and continuously updates the LCD.
2. **Intelligent Traffic Control:** A time-based state machine manages the Red (3s), Yellow (1s), and Green (3s) traffic lights. 
    * **Red Light:** Gate remains fully closed.
    * **Yellow Light:** The gate opens dynamically, mapping the elapsed time to a servo angle (0 to 90 degrees) for a smooth transition.
    * **Green Light:** Gate opens fully. If available slots drop below 2, the Green LED flashes. If the lot is completely full, the state machine locks on Red and the gate shuts.
3. **Emergency Override:** The push button is attached to a hardware interrupt pin. Pressing it instantly halts the main loop, forces the Red and Yellow LEDs off, turns the Green LED on, throws the gate wide open, and displays an emergency warning on the LCD for 5 seconds.

## Installation & Setup (PlatformIO)
This project is built using C++ and the [PlatformIO](https://platformio.org/) ecosystem. 
1. Clone this repository to your local machine.
2. Open the folder in VS Code with the PlatformIO extension installed.
3. The `platformio.ini` file will automatically download the required `Servo` and `LiquidCrystal` dependencies.
4. Click **Build** and **Upload** to flash to your Arduino Uno.

## Simulation
Don't have the hardware on hand? You can view and interact with the circuit logic in the browser:
[View the Live Simulation on TinkerCAD](https://www.tinkercad.com/things/5ExLHbw6Ioi-task1cyb063125bm0019?sharecode=MpuWPpEsLFCWLO7VDTsg15oTWtn2YtRpdwyARP_OiUo)


## Author
**Bhagabanta Giri** *B.Tech, Biomedical Engineering* **NIT Rourkela**
