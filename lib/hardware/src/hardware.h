// #ifndef utilities.h
// #define utilities.h

// #include <Arduino.h>

// volatile bool emergencyMode = false;


// #endif
#ifndef HARDWARE_H
#define HARDWARE_H

#include <Arduino.h>

void initHardware();
long readDistance(int trig, int echo);
int getAvailableSlots();
void setGate(int angle);
void setTrafficLEDs(int state, bool blinkGreen);
void updateLCD(int slots, String line1, String line2);

#endif