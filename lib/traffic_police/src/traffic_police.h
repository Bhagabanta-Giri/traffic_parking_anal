#ifndef TRAFFIC_POLICE_H
#define TRAFFIC_POLICE_H

#include <Arduino.h>

void initTrafficPolice();
void triggerEmergency();
void runTrafficLogic(int availableSlots);
int getTrafficState();
bool isEmergencyActive();

#endif