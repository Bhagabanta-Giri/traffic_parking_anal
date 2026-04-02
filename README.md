# Smart Parking and Emergency Priority Traffic System

## Introduction
In modern urban environments, inefficient parking management and rigid traffic control systems contribute significantly to congestion and delays. Drivers often spend excessive time searching for available parking spaces, while emergency vehicles like ambulances face critical delays due to unresponsive traffic signals. This project proposes a cohesive solution: a Smart Parking and Emergency Priority Traffic System. By integrating real-time parking availability tracking with an intelligent, priority-aware traffic light and gate control system, this project aims to optimize parking space utilization, reduce traffic congestion, and ensure uninterrupted passage for emergency vehicles.

## Overview of Components
* **Arduino Microcontroller:** The main processing unit that reads sensor data, manages the traffic state machine, and controls output devices.
* **Ultrasonic Sensors (1x Master Trigger, 6x Echo Pins):** Used to continuously monitor 6 individual parking slots by measuring the distance to any parked vehicle.
* **Servo Motor:** Acts as the entry gate mechanism, rotating to allow or restrict vehicle access based on traffic lights and parking availability.
* **16x2 LCD Display:** Provides a real-time, user-friendly interface displaying the current number of open parking slots and the operational status of the gate (e.g., OPENING, FULL, EMERGENCY).
* **LEDs (Red, Yellow, Green):** Simulates a real-world traffic light system to control the flow of incoming vehicles.
* **Push Button:** Acts as an emergency trigger, wired to a hardware interrupt pin to instantly override the normal system flow.

## Working Explanation
The system operates using a state machine and a hardware interrupt to coordinate three main subsystems: parking monitoring, traffic flow, and emergency response.

1. **Automated Parking Management:** A master trigger pin pulses 6 distinct ultrasonic echo pins. The distance returned determines if a car is present. The system calculates the total available slots and continuously updates the 16x2 LCD display.
2. **Intelligent Traffic and Gate Control:** A time-based state machine manages the Red (3s), Yellow (1s), and Green (3s) traffic lights. 
    * **Red Light:** Gate remains fully closed.
    * **Yellow Light:** *Custom Feature* - The gate opens dynamically and gradually, mapping the elapsed time to a servo angle (0 to 90 degrees) for a realistic transition.
    * **Green Light:** Gate opens fully (90 degrees). *Custom Feature* - If available slots drop below 2, the Green LED will flash to warn drivers of limited space. If the lot is entirely full, the gate is overridden and stays locked shut regardless of the light.
3. **Emergency Traffic Control:** The push button is attached to a hardware interrupt pin (FALLING edge). Pressing it instantly halts the main loop, forces the Red and Yellow LEDs off, turns the Green LED on, throws the gate to 90 degrees, and displays an emergency warning on the LCD for 5 seconds to allow an ambulance to pass without delay.

## TinkerCAD Simulation Link
[View the Live Simulation on TinkerCAD](https://www.tinkercad.com/things/5ExLHbw6Ioi-task1cyb063125bm0019?sharecode=MpuWPpEsLFCWLO7VDTsg15oTWtn2YtRpdwyARP_OiUo)
