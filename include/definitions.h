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
extern double wheelbaseWidth;

void driveStraight(double targetDistance /* in feet */, double speed /* percentage */);

void rotateToHeading(int targetHeading, float speed);

void pivotTurn(int turn, float speed);

void arcTurn(float theta, float radius, float speed);

int runDriveTrain();

const char* directionTypeToString(vex::directionType direction);

#endif