/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "definitions.h"

using namespace vex;

// A global instance of competition
competition Competition;

//flags
bool clamped = false;


int intakeState = 0;

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {

  task RUNDRIVETRAIN = task(runDriveTrain);
  
  while (true) {

    // intake
    if(Controller1.ButtonR1.pressing() && !intakeState == 1) {
      intakeMotor.spin(fwd, 100, pct);
      intakeState == 1;
      waitUntil(!Controller1.ButtonR2.pressing());
    }
    else if(Controller1.ButtonR2.pressing() && !intakeState == -1) {
      intakeMotor.spin(reverse, 100, pct);
      intakeState = -1;
      waitUntil(!Controller1.ButtonR2.pressing());
    }
    else if(Controller1.ButtonR1.pressing() || Controller1.ButtonR2.pressing() || Controller1.ButtonL1.pressing()) {
      intakeMotor.stop(coast);
      intakeState = 0;
    }
    
    // clamp
    if(Controller1.ButtonA.pressing() && !clamped) {
      clamp.set(true);
      clamped = true;
      waitUntil(!Controller1.ButtonA.pressing());
    }
    else if(Controller1.ButtonA.pressing() && clamped){
      clamp.set(false);
      clamped = false;
      waitUntil(!Controller1.ButtonA.pressing());
    }

    wait(20, msec);
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
