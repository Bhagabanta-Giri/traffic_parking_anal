# Project Headers & Configuration

This directory contains the global configuration for the Smart Parking System.

### Contents
- **`constants.h`**: Centralized definitions for:
  - Hardware Pins (Ultrasonic, LEDs, Servo, Button).
  - Timing intervals (Red/Yellow/Green durations).
  - Thresholds (Distance for empty/full slots).

### Usage
This file is included by both the `hardware`, `traffic_police` libraries to ensure that pin assignments remain consistent across the entire project.