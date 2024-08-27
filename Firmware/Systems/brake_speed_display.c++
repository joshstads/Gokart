//find the wokwi project here: https://wokwi.com/projects/406147662857946113
#include <LiquidCrystal_I2C.h>

// Pin definitions
const int brakeInputPin = 2;
const int brakeLightPin = 4;
const int wheelSensorPin = 5;  // Button to simulate wheel rotation

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variables for speed calculation
const float wheelCircumference = 1.5;  // Wheel circumference in meters
unsigned long lastRotationTime = 0;
float currentSpeed = 0.0;
const int numReadingsForAverage = 5;
float speedReadings[numReadingsForAverage];
int readIndex = 0;

void setup() {
  Serial.begin(115200);
  pinMode(brakeInputPin, INPUT_PULLUP);
  pinMode(brakeLightPin, OUTPUT);
  pinMode(wheelSensorPin, INPUT_PULLUP);
  
  lcd.init();
  lcd.backlight();
  lcd.print("Gokart Ready!");
  
  attachInterrupt(digitalPinToInterrupt(wheelSensorPin), wheelRotation, FALLING);
  
  // Initialize speedReadings array
  for (int i = 0; i < numReadingsForAverage; i++) {
    speedReadings[i] = 0;
  }
  
  Serial.println("Setup complete");
}

void loop() {
  handleBrakeLight();
  updateSpeed();
  updateLCD();
  delay(50);
}

void handleBrakeLight() {
  digitalWrite(brakeLightPin, !digitalRead(brakeInputPin));
}

void wheelRotation() {
  unsigned long currentTime = millis();
  if (lastRotationTime != 0) {
    float timeDiff = (currentTime - lastRotationTime) / 1000.0;  // Convert to seconds
    if (timeDiff > 0) {  // Avoid division by zero
      float instantSpeed = (wheelCircumference / timeDiff) * 3.6;  // Convert m/s to km/h
      
      // Update moving average
      speedReadings[readIndex] = instantSpeed;
      readIndex = (readIndex + 1) % numReadingsForAverage;
      
      float totalSpeed = 0;
      for (int i = 0; i < numReadingsForAverage; i++) {
        totalSpeed += speedReadings[i];
      }
      currentSpeed = totalSpeed / numReadingsForAverage;
      
      Serial.println("Speed: " + String(currentSpeed, 2) + " km/h");
    }
  }
  lastRotationTime = currentTime;
}

void updateSpeed() {
  // If no rotation detected for 3 seconds, assume speed is 0
  if (millis() - lastRotationTime > 3000) {
    currentSpeed = 0;
    // Reset speed readings
    for (int i = 0; i < numReadingsForAverage; i++) {
      speedReadings[i] = 0;
    }
    Serial.println("No rotation for 3 seconds. Speed set to 0.");
  }
}

void updateLCD() {
  lcd.setCursor(0, 1);
  lcd.print("Speed: ");
  lcd.print(currentSpeed, 1);  // Display speed with 1 decimal place
  lcd.print(" km/h    ");
}