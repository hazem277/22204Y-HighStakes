#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1  = controller(primary);

motor intakeMotor       = motor(PORT14, ratio18_1, false);

motor rightFrontMotor   = motor(PORT13, ratio18_1, false);
motor rightMiddleMotor   = motor(PORT12, ratio18_1, false);
motor rightBackMotor    = motor(PORT11, ratio18_1, false);

motor leftFrontMotor    = motor(PORT18, ratio18_1, true);
motor leftMiddleMotor    = motor(PORT19, ratio18_1, true);
motor leftBackMotor     = motor(PORT20, ratio18_1, true);

motor_group rightMotors = motor_group(rightFrontMotor, rightMiddleMotor, rightBackMotor);
motor_group leftMotors = motor_group(leftFrontMotor, leftMiddleMotor, leftBackMotor);

motor rightStakeMotor   = motor(PORT1, ratio18_1, false);
motor leftStakeMotor    = motor(PORT10, ratio18_1, true);

digital_out clamp       = digital_out(Brain.ThreeWirePort.B);
digital_out arm         = digital_out(Brain.ThreeWirePort.A);

inertial inertialSensor = inertial(PORT17);
// reciever is in port 12

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}