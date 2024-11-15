using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor intakeMotor;
extern motor rightFrontMotor;
extern motor rightBackMotor;
extern motor leftFrontMotor;
extern motor leftBackMotor;
extern digital_out clamp;
extern digital_out arm;
extern digital_in recordAuton;
extern digital_in skills;
extern digital_in positive;
extern digital_in red;
extern digital_in readAuton;
extern inertial inertialSensor;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );