#include <AFMotor.h>  // Adafruit motor shield library
#include <NewPing.h>  // Ultrasonic sensor library
#include <Servo.h>    // Servo motor library

#define TRIG_PIN A0      // Ultrasonic sensor trigger pin
#define ECHO_PIN A1      // Ultrasonic sensor echo pin
#define MAX_DISTANCE 200 // Max distance for ultrasonic sensor
#define MOTOR_SPEED 150  // Medium speed for all motors except motor3

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); // Initialize ultrasonic sensor

AF_DCMotor motor1(1, MOTOR12_1KHZ); // Left front motor
AF_DCMotor motor2(2, MOTOR12_1KHZ); // Right front motor
AF_DCMotor motor3(3, MOTOR34_1KHZ); // **Motor 3 always moves forward**
AF_DCMotor motor4(4, MOTOR34_1KHZ); // Right rear motor

Servo myservo;  // Create servo object

boolean goesForward = false; // Track forward movement state
int distance = 100; // Store distance from obstacle

void setup() {
  myservo.attach(10);  
  myservo.write(115);  // Move servo to center position
  delay(2000);

  distance = readPing(); // Read initial distance
}

void loop() {
  int distanceR = 0;
  int distanceL = 0;
  
  if (distance <= 15) { // If obstacle is detected
    moveStop();
    delay(100);
    moveBackward();
    delay(300);
    moveStop();
    delay(200);

    // Check distances on both sides
    distanceR = lookRight();
    delay(200);
    distanceL = lookLeft();
    delay(200);

    // Turn toward the direction with more space
    if (distanceR >= distanceL) {
      turnRight();
    } else {
      turnLeft();
    }
    moveStop();
  } else {
    moveForward(); // Continue moving forward if no obstacle
  }

  distance = readPing(); // Read new distance
}

// Function to check distance on the right side
int lookRight() {
  myservo.write(50);
  delay(500);
  int distance = readPing();
  myservo.write(115);
  return distance;
}

// Function to check distance on the left side
int lookLeft() {
  myservo.write(170);
  delay(500);
  int distance = readPing();
  myservo.write(115);
  return distance;
}

// Function to read distance from ultrasonic sensor
int readPing() { 
  delay(70);
  int cm = sonar.ping_cm();
  return (cm == 0) ? 250 : cm;
}

// Function to stop all motors except motor3
void moveStop() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor4.run(RELEASE);
}

// Function to move forward
void moveForward() {
  if (!goesForward) {
    goesForward = true;
    
    // Move motors forward
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    motor4.run(FORWARD);

    // Motor 3 always moves forward at medium speed
    motor3.run(FORWARD);
    motor3.setSpeed(120); // Medium speed (fixed)

    // Set speed for other motors
    motor1.setSpeed(MOTOR_SPEED);
    motor2.setSpeed(MOTOR_SPEED);
    motor4.setSpeed(MOTOR_SPEED);
  }
}

// Function to move backward (excluding motor3)
void moveBackward() {
  goesForward = false;

  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor4.run(BACKWARD);

  motor1.setSpeed(MOTOR_SPEED);
  motor2.setSpeed(MOTOR_SPEED);
  motor4.setSpeed(MOTOR_SPEED);
}

// Function to turn right (excluding motor3)
void turnRight() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor4.run(BACKWARD);
  delay(500);
}

// Function to turn left (excluding motor3)
void turnLeft() {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor4.run(FORWARD);
  delay(500);
}
