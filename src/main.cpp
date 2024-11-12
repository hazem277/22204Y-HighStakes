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

uint8_t   myarray[2][1024];

bool recordAuton = true; // set to false for normal driver control

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

  if(recordAuton) {

    Controller1.rumble("-");
    wait(1, sec);
    Controller1.rumble("-");
    wait(1, sec);
    Controller1.rumble("-");
    wait(1, sec);
    Controller1.rumble("...");

    for(int i=0;i<1024;i++) {
      myarray[0][i] = i;
      myarray[1][i] = 255-i;
    }

    Brain.Screen.printAt(20, 20, "SD Card Inserted?: %d", Brain.SDcard.isInserted());

    Brain.SDcard.savefile("test.bin", (uint8_t *)myarray, sizeof(myarray));

    memset(myarray, 0, sizeof(myarray)); // clears arrays

    Brain.SDcard.loadfile("test.bin", (uint8_t *)myarray, sizeof(myarray));

    Brain.Screen.printAt(20, 40,"%02X %02X %02X %02X", myarray[0][0], myarray[0][1], myarray[0][2],myarray[0][3]);
    Brain.Screen.printAt(20, 60,"%02X %02X %02X %02X", myarray[1][0], myarray[1][1], myarray[1][2],myarray[1][3]);

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

    wait(20, msec);
    // std::cout << "\033[3A"; // go up 3 lines
    // std::cout << "\rarm: " << arm.value() << std::endl;
    // std::cout << "\033[1B"; // go down one line
    // std::cout << "\rclamp: " << clamp.value() << std::endl;
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
