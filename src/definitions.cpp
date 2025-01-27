#include "definitions.h"
#include "vex.h"
#include <math.h>

// PID Constants
double Kp = 0.0;  // Adjust based on tuning
double Ki = 0.0;  // Typically low for heading control
double Kd = 0.0;  // Adjust based on tuning

// PID Variables
double heading_setpoint;
double error = 0;
double previous_error = 0;
double cumulative_error = 0;
double correction = 0;

double intendedHeading = 0;
double wheelCircumference = 3.25 * M_PI / 12; // ft
double wheelbaseWidth = 15; // inches

int deadband = 5;

void driveStraight(double targetDistance /*feet*/, double speed /*percentage*/) {

  // convert from feet to degrees
  double targetDegrees = targetDistance * 360 / wheelCircumference;

  // set initial heading
  heading_setpoint = intendedHeading;

  int driveDirection = targetDistance > 0 ? 1 : -1;

  // Reset encoders
  rightMotors.resetPosition();
  leftMotors.resetPosition();

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
    rightMotors.spin(driveDirection == 1 ? fwd : reverse, speed - correction, pct);
    leftMotors.spin(driveDirection == 1 ? fwd : reverse, speed + correction, pct);

    previous_error = error;
  }

  // Stop the motors
  rightMotors.stop(brake);
  leftMotors.stop(brake);
}

void rotateToHeading(int targetHeading /*degrees*/, float speed /*percentage*/) {
  intendedHeading = targetHeading;

  // Determine the current heading
  float currentHeading = inertialSensor.heading(deg);

  // Calculate the shortest path to the target heading
  float deltaHeading = fmod((targetHeading - currentHeading + 180), 360) - 180;

  // Determine the initial direction of the turn
  int turnDirection = (deltaHeading > 0) ? 1 : -1;

  while (fabs(deltaHeading) > 0.5) {
    // Recalculate current heading
    currentHeading = inertialSensor.heading(deg);

    // Calculate the new delta heading
    deltaHeading = fmod((targetHeading - currentHeading + 180), 360) - 180;

    // Adjust turn direction if overshoot occurs
    turnDirection = (deltaHeading > 0) ? 1 : -1;

    // Adjusted speed using power factor
    float powerFactor = 0.75; // Adjust the power factor for desired speed decrease rate
    float turnRatio = fabs(deltaHeading) / 180.0;
    float adjustedSpeed = speed * pow(turnRatio, powerFactor);

    // Control the motors based on the direction of the turn
    rightMotors.spin(turnDirection == 1 ? reverse : fwd, adjustedSpeed, pct);
    leftMotors.spin(turnDirection == 1 ? fwd : reverse, adjustedSpeed, pct);

    task::sleep(20);
  }

  rightMotors.stop(brake);
  leftMotors.stop(brake);

  task::sleep(50);
}

void pivotTurn(int theta /*degrees*/, float speed /*precentage*/) {
  inertialSensor.resetRotation();

  intendedHeading += theta;
  intendedHeading = fmod(intendedHeading, 360);

  int turnDirection = (theta > 0) ? 1 : -1; // Determine the direction of the turn

  while ((turnDirection == 1 && inertialSensor.rotation(deg) < theta) ||
         (turnDirection == -1 && inertialSensor.rotation(deg) > theta)) {
    float turnRatio = fabs(inertialSensor.rotation(deg)) / abs(theta);

    // Adjusted speed using power factor
    float powerFactor = 0.5; // Adjust the power factor for desired speed decrease rate
    float adjustedSpeed = speed * pow(1 - turnRatio, powerFactor);

    rightMotors.spin(turnDirection == 1 ? reverse : fwd, adjustedSpeed, pct);
    leftMotors.spin(turnDirection == 1 ? fwd : reverse, adjustedSpeed, pct);

    task::sleep(10);
  }

  rightMotors.stop(brake);
  leftMotors.stop(brake);

  rotateToHeading(intendedHeading, 50);

  task::sleep(20);
}

void arcTurn(float theta /*degrees*/, float radius /*inches*/, float speed /*precent*/) {
  inertialSensor.resetRotation();
    
  // Determine the direction of the turn
  int turnDirection = (theta > 0) ? 1 : -1;

  // Calculate the speeds for inner and outer wheels based on the radius and wheelbase width
  float innerSpeed = speed * (radius - wheelbaseWidth / 2) / radius;
  float outerSpeed = speed * (radius + wheelbaseWidth / 2) / radius;

  // Loop until the robot turns the desired angle (θ degrees)
  while ((turnDirection == 1 && inertialSensor.rotation(deg) < theta) || 
         (turnDirection == -1 && inertialSensor.rotation(deg) > -theta)) {
    rightMotors.spin(fwd, turnDirection == 1? innerSpeed : outerSpeed, pct);
    leftMotors.spin(fwd, turnDirection == 1? outerSpeed : innerSpeed, pct);
        
    task::sleep(10);
  }

  rightMotors.stop(hold);
  leftMotors.stop(hold);
    
  task::sleep(50);
}

int runDriveTrain() {
  int rightMotorSpeed = 0;
  int leftMotorSpeed = 0;
  bool rightBraked = false;
  bool leftBraked = false;

  while(true) {
    rightMotorSpeed = Controller1.Axis3.position() - Controller1.Axis1.position();
    leftMotorSpeed = Controller1.Axis3.position() + Controller1.Axis1.position();

    // right motors
    if(abs(rightMotorSpeed) > deadband) {
      rightMotors.spin(fwd, rightMotorSpeed, pct);
      rightBraked = false;
    }
    else if(!rightBraked) {
      rightMotors.stop(brake);
      rightBraked = true;
    }

    // left motors
    if(abs(leftMotorSpeed) > deadband) {
      leftMotors.spin(fwd, leftMotorSpeed, pct);
      leftBraked = false;
    }
    else if(!leftBraked) {
      leftMotors.stop(brake);
      leftBraked = true;
    }
  }
  return 0;
}

// void setBit(uint8_t *array, int index, bool value) {
//   int byteIndex = index / 8;
//   int bitIndex = index % 8;
//   if(value) {
//     array[byteIndex] |= (1 << bitIndex);
//   }
//   else {
//     array[byteIndex] &= ~(1 << bitIndex);
//   }
// }

// bool getBit(const uint8_t *array, int index) {
//   int byteIndex = index / 8;
//   int bitIndex = index % 8;
//   return (array[byteIndex] >> bitIndex) & 1;
// }