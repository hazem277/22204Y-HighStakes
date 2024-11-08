#include "definitions.h"
#include "vex.h"
#include <math.h>

// PID Constants
double Kp = 0.5;  // Adjust based on tuning
double Ki = 0.0;  // Typically low for heading control
double Kd = 0.2;  // Adjust based on tuning

// PID Variables
double heading_setpoint;
double error = 0;
double previous_error = 0;
double cumulative_error = 0;
double correction = 0;

double intendedHeading = 0;
double wheelCircumference = 3.25 * M_PI;

void driveStraight(double targetDistance /* in feet */, double speed /* percentage */) {

  // convert from feet to degrees
  double targetDegrees = targetDistance * 360 / wheelCircumference;

  // set initial heading
  heading_setpoint = intendedHeading;

  int driveDirection = targetDistance > 0 ? 1 : -1;

  // Reset encoders
  rightFrontMotor.resetPosition();
  rightBackMotor.resetPosition();
  leftFrontMotor.resetPosition();
  leftBackMotor.resetPosition();

  while ((driveDirection == 1 && rightFrontMotor.position(deg) < targetDegrees) || 
        (driveDirection == -1 && rightFrontMotor.position(deg) > targetDegrees)) {
    // Calculate error
    double current_heading = inertialSensor.heading();
    error = heading_setpoint - current_heading;
        
    // Normalize error to -180 to 180 range (avoids wrap-around issues)
    if (error > 180) error -= 360;
    if (error < -180) error += 360;

    // Integral term (accumulating error)
    cumulative_error += error;

    // Derivative term (change in error)
    double derivative = error - previous_error;

    // PID correction for heading
    correction = (Kp * error) + (Ki * cumulative_error) + (Kd * derivative);

    // Drive motors with corrected speed
    rightFrontMotor.spin(driveDirection == 1 ? fwd : reverse, speed - correction, pct);
    rightBackMotor.spin(driveDirection == 1 ? fwd : reverse, speed - correction, pct);
    leftFrontMotor.spin(driveDirection == 1 ? fwd : reverse, speed + correction, pct);
    leftBackMotor.spin(driveDirection == 1 ? fwd : reverse, speed + correction, pct);

    previous_error = error;
  }

  // Stop the motors
  rightFrontMotor.stop(brake);
  rightBackMotor.stop(brake);
  leftFrontMotor.stop(brake);
  leftBackMotor.stop(brake);
}

void rotateToHeading(int targetHeading, float speed) {
  intendedHeading = targetHeading;

  // Determine the current heading
  float currentHeading = inertialSensor.heading(deg);

  // Calculate the shortest path to the target heading
  float deltaHeading = fmod((targetHeading - currentHeading + 540), 360) - 180;

  // Determine the initial direction of the turn
  int turnDirection = (deltaHeading > 0) ? 1 : -1;

  while (fabs(deltaHeading) > 0.5) {
    // Recalculate current heading
    currentHeading = inertialSensor.heading(deg);

    // Calculate the new delta heading
    deltaHeading = fmod((targetHeading - currentHeading + 540), 360) - 180;

    // Adjust turn direction if overshoot occurs
    turnDirection = (deltaHeading > 0) ? 1 : -1;

    // Adjusted speed using power factor
    float powerFactor = 0.75; // Adjust the power factor for desired speed decrease rate
    float turnRatio = fabs(deltaHeading) / 180.0;
    float adjustedSpeed = speed * pow(turnRatio, powerFactor);

    // Control the motors based on the direction of the turn
    rightFrontMotor.spin(turnDirection == 1 ? reverse : fwd, adjustedSpeed, pct);
    rightBackMotor.spin(turnDirection == 1 ? reverse : fwd, adjustedSpeed, pct);
    leftFrontMotor.spin(turnDirection == 1 ? fwd : reverse, adjustedSpeed, pct);
    leftBackMotor.spin(turnDirection == 1 ? fwd : reverse, adjustedSpeed, pct);

    task::sleep(20);
  }

  // Stop the motors
  rightFrontMotor.stop(brake);
  rightBackMotor.stop(brake);
  leftFrontMotor.stop(brake);
  leftBackMotor.stop(brake);


  task::sleep(50);
}

void turnTo(int turn, float speed) {
  inertialSensor.resetRotation();

  intendedHeading += turn;
  intendedHeading = fmod(intendedHeading, 360);

  int turnDirection = (turn > 0) ? 1 : -1; // Determine the direction of the turn

  while ((turnDirection == 1 && inertialSensor.rotation(deg) < turn) ||
         (turnDirection == -1 && inertialSensor.rotation(deg) > turn)) {
    float turnRatio = fabs(inertialSensor.rotation(deg)) / abs(turn);

    // Adjusted speed using power factor
    float powerFactor = 0.5; // Adjust the power factor for desired speed decrease rate
    float adjustedSpeed = speed * pow(1 - turnRatio, powerFactor);

    rightFrontMotor.spin(turnDirection == 1 ? reverse : fwd, adjustedSpeed, pct);
    rightBackMotor.spin(turnDirection == 1 ? reverse : fwd, adjustedSpeed, pct);
    leftFrontMotor.spin(turnDirection == 1 ? fwd : reverse, adjustedSpeed, pct);
    leftBackMotor.spin(turnDirection == 1 ? fwd : reverse, adjustedSpeed, pct);

    task::sleep(10);
  }

  rightFrontMotor.stop(brake);
  rightBackMotor.stop(brake);
  leftFrontMotor.stop(brake);
  leftBackMotor.stop(brake);

  rotateToHeading(intendedHeading, 50);

  task::sleep(20);
}

const char* directionTypeToString(vex::directionType direction) {
  switch (direction) {
    case vex::directionType::fwd:
      return "forward";
    
    case vex::directionType::rev:
      return "reverse";

    default:
      return "null";
    }
}

int runDriveTrain() {
  int rightMotorSpeed = 0;
  int leftMotorSpeed = 0;
  bool rightBraked = false;
  bool leftBraked = false;
  while(true) {
    leftMotorSpeed = Controller1.Axis3.position() + Controller1.Axis1.position();
    rightMotorSpeed = Controller1.Axis3.position() - Controller1.Axis1.position();

    // right motors
    if(abs(rightMotorSpeed) > 5) {
      rightFrontMotor.spin(fwd, rightMotorSpeed, pct);
      rightBackMotor.spin(fwd, rightMotorSpeed, pct);
      rightBraked = false;
    }
    else if(!rightBraked) {
      rightFrontMotor.stop(brake);
      rightBackMotor.stop(brake);
      rightBraked = true;
    }

    // left motors
    if(abs(leftMotorSpeed) > 5) {
      leftFrontMotor.spin(fwd, leftMotorSpeed, pct);
      leftBackMotor.spin(fwd, leftMotorSpeed, pct);
      leftBraked = false;
    }
    else if(!leftBraked) {
      leftFrontMotor.stop(brake);
      leftBackMotor.stop(brake);
      leftBraked = true;
    }
  }
  return 0;
}