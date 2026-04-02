#include <Arduino.h>
#include <Servo.h>
#include <LiquidCrystal.h>

// --- Pin Definitions ---
// LCD Display
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

// Parking Sensors (Slots 1 & 2)
const int trig1 = A0, echo1 = A1;
const int trig2 = A2, echo2 = A3;

// Parking Sensors (Slots 3 & 4 - Sharing Pin 13 for Trigger)
const int trig3_4 = 13, echo3 = A4, echo4 = A5;

// Gate Servo
Servo gateServo;
const int servoPin = 3;

// Traffic Lights
const int greenLED = 8;
const int yellowLED = 9;
const int redLED = 10;

// Emergency Button
const int emergencyBtn = 2;

// --- Global Variables ---
volatile bool emergencyMode = false; 
unsigned long previousMillis = 0;
int trafficState = 0; // 0=Red, 1=Yellow, 2=Green

// --- Adjusted Timings (in milliseconds) ---
const unsigned long redDuration = 3000;    // Increased Red time
const unsigned long yellowDuration = 1000; // Decreased Yellow time
const unsigned long greenDuration = 3000;  // Increased Green time


// Hardware Interrupt for Instant Emergency Response
void triggerEmergency() {
  emergencyMode = true;
}


void setup() {
  lcd.begin(16, 2);
  lcd.print("System Updating");
  
  gateServo.attach(servoPin);
  gateServo.write(0); // Start closed
  
  // Initialize Sensor Pins
  pinMode(trig1, OUTPUT); pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT); pinMode(echo2, INPUT);
  pinMode(trig3_4, OUTPUT); 
  pinMode(echo3, INPUT); pinMode(echo4, INPUT);
  
  // Initialize LED Pins
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  
  // Emergency Interrupt
  pinMode(emergencyBtn, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(emergencyBtn), triggerEmergency, FALLING);
  
  delay(1500);
  lcd.clear();
}


// Function to read ultrasonic sensors reliably
long readDistance(int trig, int echo) {
  digitalWrite(trig, LOW); delayMicroseconds(2);
  digitalWrite(trig, HIGH); delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long duration = pulseIn(echo, HIGH, 30000); 
  if (duration == 0) return 999; 
  return duration * 0.034 / 2; 
}

void loop() {
  unsigned long currentMillis = millis();

  // --- 1. EMERGENCY OVERRIDE ---
  if (emergencyMode) {
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, HIGH);
    gateServo.write(90); // Throw gate wide open for emergency
    
    lcd.setCursor(0, 0); lcd.print("EMERGENCY MODE! ");
    lcd.setCursor(0, 1); lcd.print("Gate: FORCED OP ");
    
    delay(5000); // Hold for 5 seconds
    emergencyMode = false;
    previousMillis = millis(); // Reset timer so we don't jump states
    lcd.clear();
    return; 
  }

  // --- 2. PARKING SLOT CHECK (4 SLOTS) ---
  int availableSlots = 0;
  if (readDistance(trig1, echo1) > 50) availableSlots++;
  if (readDistance(trig2, echo2) > 50) availableSlots++;
  if (readDistance(trig3_4, echo3) > 50) availableSlots++;
  if (readDistance(trig3_4, echo4) > 50) availableSlots++;
  
  bool isFull = (availableSlots == 0);

  lcd.setCursor(0, 0);
  lcd.print("Slots Open: ");
  lcd.print(availableSlots);
  lcd.print("/4 ");

  // --- 3. TRAFFIC STATE MACHINE ---
  unsigned long timeElapsed = currentMillis - previousMillis;
  
  if (trafficState == 0 && timeElapsed >= redDuration) {
    trafficState = 1; // Move to Yellow
    previousMillis = currentMillis;
    timeElapsed = 0;
  } 
  else if (trafficState == 1 && timeElapsed >= yellowDuration) {
    trafficState = 2; // Move to Green
    previousMillis = currentMillis;
    timeElapsed = 0;
  } 
  else if (trafficState == 2 && timeElapsed >= greenDuration) {
    trafficState = 0; // Move back to Red
    previousMillis = currentMillis;
    timeElapsed = 0;
  }

  // Update LEDs based on current state
  digitalWrite(redLED, trafficState == 0 ? HIGH : LOW);
  digitalWrite(yellowLED, trafficState == 1 ? HIGH : LOW);
  digitalWrite(greenLED, trafficState == 2 ? HIGH : LOW);

  // --- 4. GATE LOGIC (Depends on Lot Capacity AND Traffic Light) ---
  lcd.setCursor(0, 1);
  
  if (isFull) {
    // If lot is completely full, gate strictly closes
    gateServo.write(0);
    lcd.print("Gate: LOT FULL  ");
  } 
  else {
    // Lot has space, so gate follows traffic light
    if (trafficState == 0) {
      // Red Light
      gateServo.write(0);
      lcd.print("Gate: RED STOP  ");
    } 
    else if (trafficState == 1) {
      // Yellow Light - Slow opening logic
      // Maps the elapsed time (0 to 2000ms) to a servo angle (0 to 90 degrees)
      int currentAngle = map(timeElapsed, 0, yellowDuration, 0, 90);
      gateServo.write(constrain(currentAngle, 0, 90));
      lcd.print("Gate: OPENING...");
    } 
    else if (trafficState == 2) {
      // Green Light
      gateServo.write(90);
      lcd.print("Gate: WIDE OPEN ");
    }
  }
}