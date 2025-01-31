#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "stdint.h" // to use uint8_t type alias

// PID Constants
extern double Kp;
extern double Ki;
extern double Kd;

extern double intendedHeading;
extern double wheelCircumference;
extern double wheelbaseWidth;

extern int deadband;

void driveStraight(double targetDistance /* in feet */, double speed /* percentage */);

void rotateToHeading(int targetHeading, float speed);

void pivotTurn(int turn, float speed);

void arcTurn(float theta, float radius, float speed);

int runDriveTrain();

int printDiagnostics();

void setBit(uint8_t *array, int index, bool value);

bool getBit(const uint8_t *array, int index);

#endif