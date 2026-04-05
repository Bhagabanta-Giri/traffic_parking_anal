#ifndef constants.h
#define constants.h

#include <Arduino.h>

const int mastertrig = 13; 
const int echo1 = A0;
const int echo2 = A1;
const int echo3 = A2;
const int echo4 = A3;
const int echo5 = A4;
const int echo6 = A5;

const int servoPin = 3;

const int greenLED = 8;
const int yellowLED = 9;
const int redLED = 10;

const int emergencyBtn = 2;

const unsigned long redDuration = 3000;
const unsigned long yellowDuration = 1000;
const unsigned long greenDuration = 3000;

#endif