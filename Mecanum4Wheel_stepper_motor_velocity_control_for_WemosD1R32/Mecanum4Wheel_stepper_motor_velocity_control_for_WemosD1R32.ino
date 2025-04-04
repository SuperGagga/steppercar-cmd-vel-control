#include <AccelStepper.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// =====================
// Wheel and Motor Parameters
// =====================
const float wheelRadius = 4.85; // Wheel radius (cm)
const float wheelCircumference = 2.0 * PI * wheelRadius; // Wheel circumference (cm)
const float Width = 25; // (cm)
const float Length = 26.35; // (cm)
const float robotDimension = Width + Length; // Used for rotation calculations

const int fullStepsPerRevolution = 200;       // Full steps per revolution
const int microstepFactor = 16;               // Microstepping factor (e.g., A4988)
const int stepsPerRevolution = fullStepsPerRevolution * microstepFactor; // Total steps per revolution

// Maximum linear speed (cm/s)
const float maxLinearSpeed = 100.0;
// Conversion factor from cm/s to steps/s
const float linearConversion = (float)stepsPerRevolution / wheelCircumference;
// Maximum speed in steps/s for linear speed
const float maxSpeedSteps = maxLinearSpeed * linearConversion;

// =====================
// Motor Configuration for Wemos D1 R32
// =====================
AccelStepper LeftFrontWheel(AccelStepper::DRIVER, 26, 16);   // X-axis
AccelStepper RightFrontWheel(AccelStepper::DRIVER, 25, 27);  // Y-axis
AccelStepper LeftBackWheel(AccelStepper::DRIVER, 17, 14);    // Z-axis
AccelStepper RightBackWheel(AccelStepper::DRIVER, 19, 18);   // A-axis

// Enable pin (GPIO12)
const int EN_PIN = 12;

// =====================
// Global Command Variables
// =====================
volatile float currentVx = 0.0;
volatile float currentVy = 0.0;
volatile float currentWz = 0.0;  // rad/s

// =====================
// Speed Smoothing Parameters
// =====================
float targetSpeedLF = 0.0, targetSpeedRF = 0.0, targetSpeedLB = 0.0, targetSpeedRB = 0.0;
float currentSpeedLF = 0.0, currentSpeedRF = 0.0, currentSpeedLB = 0.0, currentSpeedRB = 0.0;
const float smoothingFactor = 0.2;

// =====================
// Serial Buffer for Non-blocking Read
// =====================
const int BUFFER_SIZE = 64;
char serialBuffer[BUFFER_SIZE];
int bufferIndex = 0;

// =====================
// Command Timeout (ms)
const unsigned long commandTimeout = 100;
unsigned long lastCommandUpdate = 0;

// Convert rad/s to steps/s
float rad_s_to_steps(float rads) {
  return rads * ((float)stepsPerRevolution / (2 * PI));
}

void setup() {
  Serial.begin(115200);
  
  // Debug: Print Parameters
  Serial.print("Wheel Circumference: ");
  Serial.print(wheelCircumference);
  Serial.println(" cm");
  Serial.print("Steps per Revolution: ");
  Serial.println(stepsPerRevolution);
  Serial.print("Max Linear Speed (steps/sec): ");
  Serial.println(maxSpeedSteps);
  
  // Set Maximum Speed for Motors
  LeftFrontWheel.setMaxSpeed(maxSpeedSteps);
  RightFrontWheel.setMaxSpeed(maxSpeedSteps);
  LeftBackWheel.setMaxSpeed(maxSpeedSteps);
  RightBackWheel.setMaxSpeed(maxSpeedSteps);
  
  // Configure Motor Enable Pin (LOW = enable, HIGH = disable)
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW);
  
  // Initialize Buffer
  bufferIndex = 0;
  serialBuffer[0] = '\0';
  lastCommandUpdate = millis();
}

void loop() {
  unsigned long now = millis();
  readSerialNonBlocking();
  
  if (bufferIndex > 0 && serialBuffer[bufferIndex - 1] == '\n') {
    parseCommand(serialBuffer);
    Serial.print("Received command: ");
    Serial.println(serialBuffer);
    bufferIndex = 0;
    serialBuffer[0] = '\0';
    lastCommandUpdate = now;
  }
  
  if (now - lastCommandUpdate > commandTimeout ||
      (fabs(currentVx) < 1e-6 && fabs(currentVy) < 1e-6 && fabs(currentWz) < 1e-6)) {
    Serial.println("No valid command or command timeout. Disabling motor.");
    currentVx = 0.0;
    currentVy = 0.0;
    currentWz = 0.0;
    digitalWrite(EN_PIN, HIGH);
  } else {
    digitalWrite(EN_PIN, LOW);
  }
  
  float linearComponentLF = (currentVx - currentVy) * maxLinearSpeed * linearConversion;
  float linearComponentRF = (currentVx + currentVy) * maxLinearSpeed * linearConversion;
  float linearComponentLB = (currentVx + currentVy) * maxLinearSpeed * linearConversion;
  float linearComponentRB = (currentVx - currentVy) * maxLinearSpeed * linearConversion;
  
  float angularComponent = rad_s_to_steps(currentWz * robotDimension);
  
  targetSpeedLF = linearComponentLF - angularComponent;
  targetSpeedRF = linearComponentRF + angularComponent;
  targetSpeedLB = linearComponentLB - angularComponent;
  targetSpeedRB = linearComponentRB + angularComponent;
  
  currentSpeedLF += (targetSpeedLF - currentSpeedLF) * smoothingFactor;
  currentSpeedRF += (targetSpeedRF - currentSpeedRF) * smoothingFactor;
  currentSpeedLB += (targetSpeedLB - currentSpeedLB) * smoothingFactor;
  currentSpeedRB += (targetSpeedRB - currentSpeedRB) * smoothingFactor;
  
  LeftFrontWheel.setSpeed(currentSpeedLF);
  RightFrontWheel.setSpeed(currentSpeedRF);
  LeftBackWheel.setSpeed(currentSpeedLB);
  RightBackWheel.setSpeed(currentSpeedRB);
  
  LeftFrontWheel.runSpeed();
  RightFrontWheel.runSpeed();
  LeftBackWheel.runSpeed();
  RightBackWheel.runSpeed();
}

void readSerialNonBlocking() {
  while (Serial.available() > 0) {
    char c = Serial.read();
    Serial.print("Char received: ");
    Serial.println(c);
    if (bufferIndex < BUFFER_SIZE - 1) {
      serialBuffer[bufferIndex++] = c;
      serialBuffer[bufferIndex] = '\0';
    }
  }
}

void parseCommand(char *cmd) {
  char *token = strtok(cmd, " \n\r");
  if (token == NULL) return;
  float vx = atof(token);
  
  token = strtok(NULL, " \n\r");
  if (token == NULL) return;
  float vy = atof(token);
  
  token = strtok(NULL, " \n\r");
  if (token == NULL) return;
  float wz = atof(token);
  
  currentVx = vx;
  currentVy = vy;
  currentWz = wz;
  
  Serial.print("Parsed: vx=");
  Serial.print(currentVx);
  Serial.print(" vy=");
  Serial.print(currentVy);
  Serial.print(" wz=");
  Serial.println(currentWz);
}