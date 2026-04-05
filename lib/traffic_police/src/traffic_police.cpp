#include "traffic_police.h"
#include "constants.h"

volatile bool emergencyMode = false;
int trafficState = 0; 
unsigned long previousMillis = 0;

void initTrafficPolice() {
    attachInterrupt(digitalPinToInterrupt(emergencyBtn), triggerEmergency, FALLING);
}

void triggerEmergency() { emergencyMode = true; }

void runTrafficLogic(int availableSlots) {
    unsigned long currentMillis = millis();
    
    if (emergencyMode) {
        trafficState = 0; 
        digitalWrite(redLED, HIGH);
        digitalWrite(yellowLED, LOW);
        digitalWrite(greenLED, LOW);
        previousMillis = currentMillis;
        return;
    }

    bool isFull = (availableSlots == 0);

    if (trafficState == 0 && currentMillis - previousMillis >= redDuration && !isFull) {
        trafficState = 1; 
        previousMillis = currentMillis;
    }
    else if (trafficState == 1 && currentMillis - previousMillis >= yellowDuration) {
        trafficState = 2; 
        previousMillis = currentMillis;
    } 
    else if (trafficState == 2 && currentMillis - previousMillis >= greenDuration) {
        trafficState = 0; 
        previousMillis = currentMillis;
    }

    digitalWrite(redLED, trafficState == 0 ? HIGH : LOW);
    digitalWrite(yellowLED, trafficState == 1 ? HIGH : LOW);
    
    if (trafficState == 2) {
        if (availableSlots > 2) {
            digitalWrite(greenLED, HIGH);
        } 
        else if (availableSlots > 0) {
            if ((currentMillis / 250) % 2 == 0) {
                digitalWrite(greenLED, HIGH);
            } 
            else {
                digitalWrite(greenLED, LOW);
            }
        }
    } 
    else {
        digitalWrite(greenLED, LOW);
    }
}

int getTrafficState() { return trafficState; }
bool isEmergencyActive() { return emergencyMode; }