#include "Hardware.h"
#include "constants.h"
#include <Servo.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
Servo gateServo;

void initHardware() {
    lcd.begin(16, 2);
    gateServo.attach(servoPin);
    pinMode(mastertrig, OUTPUT);
    pinMode(echo1, INPUT);
    pinMode(echo2, INPUT);
    pinMode(echo3, INPUT);
    pinMode(echo4, INPUT);
    pinMode(echo5, INPUT);
    pinMode(echo6, INPUT);
    pinMode(greenLED, OUTPUT);
    pinMode(redLED, OUTPUT);
    pinMode(yellowLED, OUTPUT);
    pinMode(emergencyBtn, INPUT_PULLUP);
}

long readDistance(int trig, int echo) {
    digitalWrite(trig, LOW); delayMicroseconds(2);
    digitalWrite(trig, HIGH); delayMicroseconds(10);
    digitalWrite(trig, LOW);
    long duration = pulseIn(echo, HIGH, 30000); 
    return (duration == 0) ? 999 : (duration * 0.034 / 2);
}

int getAvailableSlots() {
    int count = 0;
    if (readDistance(mastertrig, echo1) > distanceThreshold) count++;
    if (readDistance(mastertrig, echo2) > distanceThreshold) count++;
    if (readDistance(mastertrig, echo3) > distanceThreshold) count++;
    if (readDistance(mastertrig, echo4) > distanceThreshold) count++;
    if (readDistance(mastertrig, echo5) > distanceThreshold) count++;
    if (readDistance(mastertrig, echo6) > distanceThreshold) count++;
    return count;
}

void setGate(int angle) { gateServo.write(angle); }

void setTrafficLEDs(int state, bool blinkGreen) {
    digitalWrite(redLED, state == 0 ? HIGH : LOW);
    digitalWrite(yellowLED, state == 1 ? HIGH : LOW);
}

void updateLCD(int slots, String line1, String line2) {
    lcd.setCursor(0,0); lcd.print(line1);
    lcd.setCursor(0,1); lcd.print(line2);
}