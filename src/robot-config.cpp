#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1  = controller(primary);

motor intakeMotor       = motor(PORT11, ratio18_1, false);

motor rightFrontMotor   = motor(PORT14, ratio18_1, false);
motor rightMidleMotor   = motor(PORT16, ratio18_1, false);
motor rightBackMotor    = motor(PORT13, ratio18_1, false);
motor leftFrontMotor    = motor(PORT12, ratio18_1, true);
motor leftMidleMotor    = motor(PORT17, ratio18_1, true);
motor leftBackMotor     = motor(PORT20, ratio18_1, true);

motor rightStakeMotor   = motor(PORT1, ratio18_1, false);
motor leftStakeMotor    = motor(PORT1, ratio18_1, true);

digital_out clamp       = digital_out(Brain.ThreeWirePort.H);
digital_out arm         = digital_out(Brain.ThreeWirePort.G);

inertial inertialSensor = inertial(PORT15);
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