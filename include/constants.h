#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Arduino.h>

const int mastertrig = 13; 
const int echo1 = A0, echo2 = A1, echo3 = A2, echo4 = A3, echo5 = A4, echo6 = A5;
const int servoPin = 3;
const int greenLED = 8, yellowLED = 9, redLED = 10;
const int emergencyBtn = 2;

const unsigned long redDuration = 3000;
const unsigned long yellowDuration = 1000;
const unsigned long greenDuration = 3000;
const int distanceThreshold = 50; 

#endif