#include "definitions.h"
#include "vex.h"

void moveStraight(int speed, int dist, bool Reverse) {
  rightFrontMotor.setVelocity(speed, percent);
  rightBackMotor.setVelocity(speed, percent);
  leftFrontMotor.setVelocity(speed, percent);
  leftBackMotor.setVelocity(speed, percent);
  if(!Reverse){
    rightFrontMotor.spinFor(fwd, dist, deg, false);
    rightBackMotor.spinFor(fwd, dist, deg, false);
    leftFrontMotor.spinFor(fwd, dist, deg, false);
    leftBackMotor.spinFor(fwd, dist, deg);
  }
  else{
    rightFrontMotor.spinFor(reverse, dist, deg, false);
    rightBackMotor.spinFor(reverse, dist, deg, false);
    leftFrontMotor.spinFor(reverse, dist, deg, false);
    leftBackMotor.spinFor(reverse, dist, deg);
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