#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor intakeMotor = motor(PORT7, ratio18_1, false);
motor rightFrontMotor = motor(PORT13, ratio18_1, false);
motor rightBackMotor = motor(PORT6, ratio18_1, false);
motor leftFrontMotor = motor(PORT11, ratio18_1, true);
motor leftBackMotor = motor(PORT9, ratio18_1, true);
digital_out clamp = digital_out(Brain.ThreeWirePort.A);
digital_out arm = digital_out(Brain.ThreeWirePort.B);
digital_in recordAuton = digital_in(Brain.ThreeWirePort.H);
digital_in skills = digital_in(Brain.ThreeWirePort.G);
digital_in positive = digital_in(Brain.ThreeWirePort.F);
digital_in red = digital_in(Brain.ThreeWirePort.E);
digital_in readAuton = digital_in(Brain.ThreeWirePort.D);
inertial inertialSensor = inertial(PORT1);
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