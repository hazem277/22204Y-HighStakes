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
#include <stdlib.h>
#include <iostream>

using namespace vex;

// A global instance of competition
competition Competition;

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
  driveStraight(2, 50);
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
  
  std::cout << "clamp: " << clamp.value() << std::endl;
  std::cout << "arm: " << arm.value() << std::endl;

  while (true) {

    // intake
    if(Controller1.ButtonR1.pressing() && intakeMotor.velocity(pct) <= 0) {
      intakeMotor.spin(fwd, 100, pct);
      waitUntil(!Controller1.ButtonR1.pressing());
    }
    else if(Controller1.ButtonR2.pressing() && intakeMotor.velocity(pct) >= 0) {
      intakeMotor.spin(reverse, 100, pct);
      waitUntil(!Controller1.ButtonR2.pressing());
    }
    else if(Controller1.ButtonR1.pressing() || Controller1.ButtonR2.pressing() || Controller1.ButtonL1.pressing()) {
      intakeMotor.stop(coast);
      waitUntil(!(Controller1.ButtonR1.pressing() || Controller1.ButtonR2.pressing()));
    }
    
    // clamp
    if(Controller1.ButtonA.pressing() && !clamp.value()) {
      clamp.set(true);
      waitUntil(!Controller1.ButtonA.pressing());
    }
    else if(Controller1.ButtonA.pressing() && clamp.value()){
      clamp.set(false);
      waitUntil(!Controller1.ButtonA.pressing());
    }

    // arm
    if(Controller1.ButtonB.pressing() && !arm.value()) {
      arm.set(true);
      waitUntil(!Controller1.ButtonA.pressing());
    }
    else if(Controller1.ButtonB.pressing() && arm.value()){
      arm.set(false);
      waitUntil(!Controller1.ButtonB.pressing());
    }

    wait(20, msec);
    std::cout << "\033[3A"; // go up 3 lines
    std::cout << "\rarm: " << arm.value() << std::endl;
    std::cout << "\033[1B"; // go down one line
    std::cout << "\rclamp: " << clamp.value() << std::endl;
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
