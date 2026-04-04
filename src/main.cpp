#include <Arduino.h>
#include <Servo.h>
#include <LiquidCrystal.h>


LiquidCrystal lcd(12, 11, 7, 6, 5, 4);


const int mastertrig = 13; 
const int echo1 = A0;
const int echo2 = A1;
const int echo3 = A2;
const int echo4 = A3;
const int echo5 = A4;
const int echo6 = A5;

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
  pinMode(mastertrig, OUTPUT); 
  pinMode(echo1, INPUT);
  pinMode(echo2, INPUT);
  pinMode(echo3, INPUT);
  pinMode(echo4, INPUT);
  pinMode(echo5, INPUT);
  pinMode(echo6, INPUT);
  

  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  


  pinMode(emergencyBtn, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(emergencyBtn), triggerEmergency, FALLING);
  
  delay(1500);
  lcd.clear();
}


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

  int availableSlots = 0;
  if (readDistance(mastertrig, echo1) > 50) availableSlots++;
  if (readDistance(mastertrig, echo2) > 50) availableSlots++;
  if (readDistance(mastertrig, echo3) > 50) availableSlots++;
  if (readDistance(mastertrig, echo4) > 50) availableSlots++;
  if (readDistance(mastertrig, echo5) > 50) availableSlots++;
  if (readDistance(mastertrig, echo6) > 50) availableSlots++;
  
  bool isFull = (availableSlots == 0);

  lcd.setCursor(0, 0);
  lcd.print("Slots Open: ");
  lcd.print(availableSlots);
  lcd.print("/6 ");


  unsigned long timeElapsed = currentMillis - previousMillis;
  
  if (trafficState == 0 && timeElapsed >= redDuration && !isFull) {
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
  if (trafficState == 2) {
    if (availableSlots > 2) { // Only show green if there are at least 3 slots available
      digitalWrite(greenLED, HIGH);
    } 
    if (availableSlots <= 2 && availableSlots > 0) {
      if ((millis() / 250) % 2 == 0) {
        digitalWrite(greenLED, HIGH); // Blink green to indicate limited availability
      } 
      else {
        digitalWrite(greenLED, LOW);
      }
    }
  } 
  else {
    digitalWrite(greenLED, LOW);
  }

  lcd.setCursor(0, 1);
  
  if (isFull) {
    gateServo.write(0);
    lcd.print("Gate: LOT FULL  ");
  } 
  else {    if (trafficState == 0) {
      // Red Light
      gateServo.write(0);
      lcd.print("Gate: RED STOP  ");
    } 
    else if (trafficState == 1) {
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