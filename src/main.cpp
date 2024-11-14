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
#include <math.h>

using namespace vex;

// A global instance of competition
competition Competition;

enum Side {
  positive,
  negative
};

bool skills = false;
bool readAuton = true;
Side side = positive;

int8_t axisData[2][750]; //  [1] = Axis 3 | [2] = Axis 1

bool recordAuton = true; // set to false for normal driver control
double autonTimer;
int i = 0;

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
  if(readAuton) {
    if(skills) {

    }
    else if(side == positive){
      memset(axisData, 0, sizeof(axisData)); // clears arrays
      Brain.SDcard.loadfile("positive.bin", (uint8_t *)axisData, sizeof(axisData));
      for(int i = 0; i < 750; i++) {
        rightFrontMotor.spin(fwd, axisData[0][i] - axisData[1][i], pct);
        rightBackMotor.spin(fwd, axisData[0][i] - axisData[1][i], pct);
        leftFrontMotor.spin(fwd, axisData[0][i] + axisData[1][i], pct);
        leftBackMotor.spin(fwd, axisData[0][i] + axisData[1][i], pct);
        wait(20,msec);
      }
    }
    else if (side == negative) {

    }
  }
  else if(skills) {
    driveStraight(-0.5, 50);
    clamp.set(true);
    rotateToHeading(270, 50);
    intakeMotor.spin(fwd, 100, pct);
    driveStraight(1.5, 20);
    driveStraight(-1, 20);
    wait(3, sec);
    pivotTurn(90, 50);
    intakeMotor.stop();
    // driveStraight(0.5, 25);
    // rotateToHeading(145, 50);
    // clamp.set(false);
    // driveStraight(-0.5, 25);
  }
  else if(side == positive){
    driveStraight(-2, 50);
    clamp.set(true);
    intakeMotor.spin(fwd, 100, pct);
  }
  else if(side == negative) {
    driveStraight(-2, 50);
    clamp.set(true);
    intakeMotor.spin(fwd, 100, pct);
  }
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
  std::cout << "user controll started" << std::endl;

  if(recordAuton) {
    Brain.Screen.printAt(20, 20, "SD Card Inserted?: %d", Brain.SDcard.isInserted());
    Brain.Screen.printAt(20, 40, "Save file exists?: %d", Brain.SDcard.exists("controllerAxisValues.bin"));
    Brain.Screen.printAt(20, 60, "size of file: %d", Brain.SDcard.size("controllerAxisValues.bin"));

    Controller1.rumble("-");
    wait(1, sec);
    Controller1.rumble("-");
    wait(1, sec);
    Controller1.rumble("-");
    wait(1, sec);
    Controller1.rumble("...");

    autonTimer = Brain.timer(msec);

  }

  task RUNDRIVETRAIN = task(runDriveTrain);
  
  // std::cout << "\033[2k";
  // std::cout << "clamp: " << clamp.value() << std::endl;
  // std::cout << "arm: " << arm.value() << std::endl;

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
    if(Controller1.ButtonA.pressing()) {
      clamp.set(!clamp.value());
      waitUntil(!Controller1.ButtonA.pressing());
    }

    // arm
    if(Controller1.ButtonB.pressing()) {
      arm.set(!arm.value());
      waitUntil(!Controller1.ButtonB.pressing());
    }

    if(recordAuton) {
      axisData[0][i] = Controller1.Axis3.position();
      axisData[1][i] = Controller1.Axis1.position();
      std::cout << i << std::endl;
      i++;
      if(Brain.timer(msec) - autonTimer >= 15000 || i >= 749) {
        break;
      }
    }

    wait(20, msec);
    // std::cout << "\033[3A"; // go up 3 lines
    // std::cout << "\rarm: " << arm.value() << std::endl;
    // std::cout << "\033[1B"; // go down one line
    // std::cout << "\rclamp: " << clamp.value() << std::endl;
  }
  
  RUNDRIVETRAIN.stop();
  
  std::cout << "recording ended" << std::endl;
  
  Brain.SDcard.savefile("positive.bin", (uint8_t *)axisData, sizeof(axisData));
  
  Brain.Screen.printAt(20, 80, "Auton Timer: %d", Brain.SDcard.size("controllerAxisValues.bin"));
  
  rightFrontMotor.stop(brake);
  rightBackMotor.stop(brake);
  leftFrontMotor.stop(brake);
  leftBackMotor.stop(brake);
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
