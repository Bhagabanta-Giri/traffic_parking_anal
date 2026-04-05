// #include <Arduino.h>
// #include <Servo.h>
// #include <LiquidCrystal.h>
// #include "constants.h"

// LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

// Servo gateServo;

// volatile bool emergencyMode = false; 
// unsigned long previousMillis = 0;
// int trafficState = 0;


// void triggerEmergency() {
//   emergencyMode = true;
// }


// void setup() {
//   lcd.begin(16, 2);
//   lcd.print("System Updating");
  
//   gateServo.attach(servoPin);
//   gateServo.write(0);
  

//   pinMode(mastertrig, OUTPUT); 
//   pinMode(echo1, INPUT);
//   pinMode(echo2, INPUT);
//   pinMode(echo3, INPUT);
//   pinMode(echo4, INPUT);
//   pinMode(echo5, INPUT);
//   pinMode(echo6, INPUT);
  

//   pinMode(greenLED, OUTPUT);
//   pinMode(yellowLED, OUTPUT);
//   pinMode(redLED, OUTPUT);
  


//   pinMode(emergencyBtn, INPUT_PULLUP);
//   attachInterrupt(digitalPinToInterrupt(emergencyBtn), triggerEmergency, FALLING);
  
//   delay(1500);
//   lcd.clear();
// }


// long readDistance(int trig, int echo) {
//   digitalWrite(trig, LOW); delayMicroseconds(2);
//   digitalWrite(trig, HIGH); delayMicroseconds(10);
//   digitalWrite(trig, LOW);
//   long duration = pulseIn(echo, HIGH, 30000); 
//   if (duration == 0) return 999; 
//   return duration * 0.034 / 2; 
// }

// void loop() {
//   unsigned long currentMillis = millis();

//   if (emergencyMode) {
//     digitalWrite(redLED, LOW);
//     digitalWrite(yellowLED, LOW);
//     digitalWrite(greenLED, HIGH);
//     gateServo.write(90);
    
//     lcd.setCursor(0, 0); lcd.print("EMERGENCY MODE! ");
//     lcd.setCursor(0, 1); lcd.print("Gate: FORCED OP ");
    
//     delay(5000);
//     emergencyMode = false;
//     previousMillis = millis();
//     lcd.clear();
//     return; 
//   }

//   int availableSlots = 0;
//   if (readDistance(mastertrig, echo1) > 50) availableSlots++;
//   if (readDistance(mastertrig, echo2) > 50) availableSlots++;
//   if (readDistance(mastertrig, echo3) > 50) availableSlots++;
//   if (readDistance(mastertrig, echo4) > 50) availableSlots++;
//   if (readDistance(mastertrig, echo5) > 50) availableSlots++;
//   if (readDistance(mastertrig, echo6) > 50) availableSlots++;
  
//   bool isFull = (availableSlots == 0);

//   lcd.setCursor(0, 0);
//   lcd.print("Slots Open: ");
//   lcd.print(availableSlots);
//   lcd.print("/6 ");


//   unsigned long timeElapsed = currentMillis - previousMillis;
  
//   if (trafficState == 0 && timeElapsed >= redDuration && !isFull) {
//     trafficState = 1; // Move to Yellow
//     previousMillis = currentMillis;
//     timeElapsed = 0;
//   }
//   else if (trafficState == 1 && timeElapsed >= yellowDuration) {
//     trafficState = 2; // Move to Green
//     previousMillis = currentMillis;
//     timeElapsed = 0;
//   } 
//   else if (trafficState == 2 && timeElapsed >= greenDuration) {
//     trafficState = 0; // Move back to Red
//     previousMillis = currentMillis;
//     timeElapsed = 0;
//   }

//   digitalWrite(redLED, trafficState == 0 ? HIGH : LOW);
//   digitalWrite(yellowLED, trafficState == 1 ? HIGH : LOW);
//   if (trafficState == 2) {
//     if (availableSlots > 2) {
//       digitalWrite(greenLED, HIGH);
//     } 
//     if (availableSlots <= 2 && availableSlots > 0) {
//       if ((millis() / 250) % 2 == 0) {
//         digitalWrite(greenLED, HIGH);
//       } 
//       else {
//         digitalWrite(greenLED, LOW);
//       }
//     }
//   } 
//   else {
//     digitalWrite(greenLED, LOW);
//   }

//   lcd.setCursor(0, 1);
  
//   if (isFull) {
//     gateServo.write(0);
//     lcd.print("Gate: LOT FULL  ");
//   } 
//   else {    if (trafficState == 0) {
//       // Red Light
//       gateServo.write(0);
//       lcd.print("Gate: RED STOP  ");
//     } 
//     else if (trafficState == 1) {
//       int currentAngle = map(timeElapsed, 0, yellowDuration, 0, 90);
//       gateServo.write(constrain(currentAngle, 0, 90));
//       lcd.print("Gate: OPENING...");
//     } 
//     else if (trafficState == 2) {
//       // Green Light
//       gateServo.write(90);
//       lcd.print("Gate: WIDE OPEN ");
//     }
//   }
// }
#include <Arduino.h>
#include <Hardware.h>
#include "constants.h"
#include "traffic_police.h"

void setup() {
    initHardware();
    initTrafficPolice();
}

void loop() {
    int slots = getAvailableSlots();
    
    runTrafficLogic(slots); 
    
    int state = getTrafficState();
    setTrafficLEDs(state, (slots <= 2)); 
    
    if (isEmergencyActive()) {
        updateLCD(slots, "EMERGENCY MODE!", "Gate: FORCED OP ");
        setGate(90);
    } else {
        if (state == 0) {
            setGate(0);
            updateLCD(slots, "Slots Open: " + String(slots) + "/6", "Gate: CLOSED(RED)");
        } else if (state == 1) {
            unsigned long timeElapsed = millis() % yellowDuration;
            int angle = map(timeElapsed, 0, yellowDuration, 0, 90);
            setGate(constrain(angle, 0, 90));
            updateLCD(slots, "Slots Open: " + String(slots) + "/6", "Gate: OPENING(YELLOW)");
        } else if (state == 2) {
            setGate(90);
            updateLCD(slots, "Slots Open: " + String(slots) + "/6", "Gate: OPEN(GREEN)");
        }
    }
}