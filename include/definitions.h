#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// PID Constants
extern double Kp;
extern double Ki;
extern double Kd;

// PID Variables
extern double heading_setpoint;
extern double error;
extern double previous_error;
extern double cumulative_error;
extern double correction;

extern double intendedHeading;
extern double wheelCircumference;

void driveStraight(double targetDistance /* in feet */, double speed /* percentage */);

void rotateTo(int targetHeading, float speed);

void turnTo(int turn, float speed);

int runDriveTrain();

#endif