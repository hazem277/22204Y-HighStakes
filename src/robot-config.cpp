#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1  = controller(primary);

motor intakeMotor       = motor(PORT5, ratio18_1, true);

motor rightFrontMotor   = motor(PORT1, ratio18_1, false);
motor rightMiddleMotor   = motor(PORT2, ratio18_1, false);
motor rightBackMotor    = motor(PORT3, ratio18_1, false);

motor leftFrontMotor    = motor(PORT10, ratio18_1, true);
motor leftMiddleMotor    = motor(PORT9, ratio18_1, true);
motor leftBackMotor     = motor(PORT8, ratio18_1, true);

motor_group rightMotors = motor_group(rightFrontMotor, rightMiddleMotor, rightBackMotor);
motor_group leftMotors = motor_group(leftFrontMotor, leftMiddleMotor, leftBackMotor);

motor rightStakeMotor   = motor(PORT4, ratio18_1, false);
motor leftStakeMotor    = motor(PORT7, ratio18_1, true);
motor_group stakeMotors = motor_group(rightStakeMotor, leftStakeMotor);

digital_out clamp       = digital_out(Brain.ThreeWirePort.A);
digital_out arm         = digital_out(Brain.ThreeWirePort.B);

inertial inertialSensor = inertial(PORT6);
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