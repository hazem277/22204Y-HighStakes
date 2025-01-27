using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;

extern motor intakeMotor;

extern motor rightFrontMotor;
extern motor rightMiddleMotor;
extern motor rightBackMotor;
extern motor leftFrontMotor;
extern motor leftMiddleMotor;
extern motor leftBackMotor;

extern motor_group rightMotors;
extern motor_group leftMotors;

extern motor rightStakeMotor;
extern motor leftStakeMotor;

extern digital_out clamp;
extern digital_out arm;

extern inertial inertialSensor;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );