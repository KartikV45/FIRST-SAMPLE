#include <ESP32Servo.h>

Servo myServo1; // First Servo object
Servo myServo2; // Second Servo object

const int soundSensorPin1 = 34; // Sound sensor for the first servo
const int soundSensorPin2 = 32; // Sound sensor for the second servo
const int moistureSensorPin = 35; // Soil moisture sensor for controlling the servos

int thresholdHigh = 800;  // Sound level above which the servo should activate
int thresholdLow = 500;   // Sound level below which the servo should stop
int moistureThreshold = 600; // Threshold for soil moisture, adjust based on calibration
int moistureLevel = 0;    // Store the current moisture level

int pos1 = 70; // Initial position for the first servo
int pos2 = 70; // Initial position for the second servo
int increment1 = 1; // Step size for the first servo movement
int increment2 = -1; // Step size for the second servo movement (opposite direction)

bool servoActive1 = false; // Flag to indicate whether the first servo should move
bool servoActive2 = false; // Flag to indicate whether the second servo should move

unsigned long startMillis = 0; // Timer to track the 30 seconds of rotation
unsigned long rotateDuration = 30000; // 30 seconds in milliseconds

void setup() {
  myServo1.attach(13); // Attach the first servo to GPIO 13
  myServo2.attach(12); // Attach the second servo to GPIO 12

  pinMode(soundSensorPin1, INPUT); // Initialize the first sound sensor pin
  pinMode(soundSensorPin2, INPUT); // Initialize the second sound sensor pin
  pinMode(moistureSensorPin, INPUT); // Initialize the moisture sensor pin

  Serial.begin(115200); // Start serial communication for debugging
}

void loop() {
  int soundLevel1 = analogRead(soundSensorPin1); // Read the first sound sensor
  int soundLevel2 = analogRead(soundSensorPin2); // Read the second sound sensor
  moistureLevel = analogRead(moistureSensorPin); // Read the moisture sensor

  Serial.print("Sound Level 1: ");
  Serial.print(soundLevel1);
  Serial.print("\tSound Level 2: ");
  Serial.println(soundLevel2);
  Serial.print("\tMoisture Level: ");
  Serial.println(moistureLevel);

  // Check if the first sound sensor is above the threshold and activate the corresponding servo
  if (soundLevel1 > thresholdHigh && !servoActive1) {
    servoActive1 = true; // Start first servo movement
    startMillis = millis(); // Start the timer for 30 seconds
    Serial.println("Sound detected on Sensor 1! Starting rotation for Servo 1.");
  }

  // Check if the second sound sensor is above the threshold and activate the corresponding servo
  if (soundLevel2 > thresholdHigh && !servoActive2) {
    servoActive2 = true; // Start second servo movement
    startMillis = millis(); // Start the timer for 30 seconds
    Serial.println("Sound detected on Sensor 2! Starting rotation for Servo 2.");
  }

  // If the first servo is active, move it
  if (servoActive1) {
    myServo1.write(pos1);
    pos1 += increment1;
    if (pos1 >= 110 || pos1 <= 30) {
      increment1 = -increment1; // Reverse direction when limits are reached
    }

    // Stop after 30 seconds
    if (millis() - startMillis >= rotateDuration) {
      servoActive1 = false;
      myServo1.write(90); // Center the servo at 90°
      Serial.println("30 seconds passed. Stopping Servo 1.");
    }
  }

  // If the second servo is active, move it
  if (servoActive2) {
    myServo2.write(pos2);
    pos2 -= increment2; // Move in the opposite direction of Servo 1
    if (pos2 >= 110 || pos2 <= 30) {
      increment2 = -increment2; // Reverse direction when limits are reached
    }

    // Stop after 30 seconds
    if (millis() - startMillis >= rotateDuration) {
      servoActive2 = false;
      myServo2.write(90); // Center the servo at 90°
      Serial.println("30 seconds passed. Stopping Servo 2.");
    }
  }

  // If the moisture level is low and no sound is detected, stop all servos
  if (moistureLevel < moistureThreshold && !servoActive1 && !servoActive2) {
    myServo1.write(90); // Center all servos
    myServo2.write(90);
    Serial.println("Soil moisture is low! All servos stopped.");
  }

  delay(20); // Small delay to smooth servo movement
}