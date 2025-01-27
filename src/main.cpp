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

/*auton settings_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_*/
enum Side {                                         /* makes code more readable                */
  POSITIVE,                                         /* used for auton positive corner setup    */
  NEGATIVE };                                       /* used for auton negative corner setup    */
/*---------------------------------------------------------------------------------------------*/
/* auton setup---------------------------------------------------------------------------------*/
bool recordAuton = false;                           /* set to false for normal driver control  */
bool readAuton = false;                             /* first priority                          */
bool skills = true;                                /* second priority                         */
Side side = POSITIVE;                               /* last priority                           */
double autonTimer;                                  /* time auton started recording            */
int autonIndex = 0;                                 /* index for recorded values               */
/*---------------------------------------------------------------------------------------------*/
/*arrays for storing recorded data-------------------------------------------------------------*/
int8_t  axisData[2][6000] = {0};                    /* [1] = Axis 3 | [2] = Axis 1             */
uint8_t clampData[750] = {0};                       /* 750 bytes * 8 bits = 6000 bits          */
uint8_t armData[750] = {0};                         /* 1 = on | 0 = off                        */
int8_t intakeData[6000] = {0};                      /* 1 = fwd | 0 = off | -1 = reverse        */
/*_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_*/


void setBit(uint8_t *array, int index, bool value) {
  int byteIndex = index / 8;
  int bitIndex = index % 8;
  if(value) {
    array[byteIndex] |= (1 << bitIndex);
  }
  else {
    array[byteIndex] &= ~(1 << bitIndex);
  }
}

bool getBit(const uint8_t *array, int index) {
  int byteIndex = index / 8;
  int bitIndex = index % 8;
  return (array[byteIndex] >> bitIndex) & 1;
}

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
      // clear arrays
      memset(axisData, 0, sizeof(axisData));
      memset(clampData, 0, sizeof(clampData));
      memset(armData, 0, sizeof(armData));
      memset(intakeData, 0, sizeof(intakeData));
    if(skills) {
      Brain.SDcard.loadfile("skillsAxisData.bin", (uint8_t *)axisData, sizeof(axisData));
      Brain.SDcard.loadfile("skillsClampData.bin", (uint8_t *)clampData, sizeof(clampData));
      Brain.SDcard.loadfile("skillsArmData.bin", (uint8_t *)armData, sizeof(armData));
      Brain.SDcard.savefile("skillsIntakeData.bin", (uint8_t *)intakeData, sizeof(intakeData));
    }
    else if(side == POSITIVE){
      Brain.SDcard.loadfile("positiveAxisData.bin", (uint8_t *)axisData, sizeof(axisData));
      Brain.SDcard.loadfile("positiveClampData.bin", (uint8_t *)clampData, sizeof(clampData));
      Brain.SDcard.loadfile("positiveArmData.bin", (uint8_t *)armData, sizeof(armData));
      Brain.SDcard.savefile("positiveIntakeData.bin", (uint8_t *)intakeData, sizeof(intakeData));
    }
    else if (side == POSITIVE) {
      Brain.SDcard.loadfile("negativeAxisData.bin", (uint8_t *)axisData, sizeof(axisData));
      Brain.SDcard.loadfile("negativeClampData.bin", (uint8_t *)clampData, sizeof(clampData));
      Brain.SDcard.loadfile("negativeArmData.bin", (uint8_t *)armData, sizeof(armData));
      Brain.SDcard.savefile("negativeIntakeData.bin", (uint8_t *)intakeData, sizeof(intakeData));
    }
    for(int index = 0; index < 750; index++) {
      rightMotors.spin(fwd, axisData[0][index] - axisData[1][index], pct);
      leftMotors.spin(fwd, axisData[0][index] + axisData[1][index], pct);
      if(getBit(clampData, index) != clamp.value()) {
        clamp.set(!clamp.value());
      }
      if(getBit(armData, index) != arm.value()) {
        arm.set(!arm.value());
      }
      if(intakeData[index] == 1) {
        intakeMotor.spin(fwd, 100, pct);
      }
      else if(intakeData[index] == -1) {
        intakeMotor.spin(reverse, 100, pct);
      }
      else if(intakeData[index] == 0) {
        intakeMotor.stop(coast);
      }
      wait(20,msec);
    }
  }
  else if(skills) {
    driveStraight(-0.29, 75);
  }
  else if(side == POSITIVE){
    driveStraight(-1.75, 50);
  }
  else if(side == NEGATIVE) {
    driveStraight(-1.5, 50);
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

    std::cout << "autonTimer: " << autonTimer << std::endl;

  }

  task RUNDRIVETRAIN = task(runDriveTrain);
  
  // std::cout << "\033[2k";
  // std::cout << "clamp: " << clamp.value() << std::endl;
  // std::cout << "arm: " << arm.value() << std::endl;

  while (true) {

    int timeStamp = Brain.timer(msec) - autonTimer;

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

    // stake arm

    if(Controller1.ButtonUp.pressing() && Controller1.ButtonDown.pressing()) {
      rightStakeMotor.stop(hold);
      leftStakeMotor.stop(hold);
    }
    else if(Controller1.ButtonUp.pressing()) {
      rightStakeMotor.spin(fwd, 100, pct);
      leftStakeMotor.spin(fwd, 100, pct);
    }
    else if(Controller1.ButtonDown.pressing()) {
      rightStakeMotor.spin(reverse, 100, pct);
      leftStakeMotor.spin(reverse, 100, pct);
    }
    else {
      rightStakeMotor.stop(hold);
      leftStakeMotor.stop(hold);
    }

    if(recordAuton) {
      axisData[0][autonIndex] = Controller1.Axis3.position();
      axisData[1][autonIndex] = Controller1.Axis1.position();
      setBit(clampData, autonIndex, clamp.value());
      setBit(armData, autonIndex, arm.value());
      if(intakeMotor.velocity(pct) == 0) {
        intakeData[autonIndex] = 0;
      }
      else if(intakeMotor.velocity(pct) > 0) {
        intakeData[autonIndex] = 1;
      }
      else if(intakeMotor.velocity(pct) < 0) {
        intakeData[autonIndex] = -1;
      }
      std::cout << autonIndex << std::endl;
      if((Brain.timer(msec) - autonTimer >= 750 && !skills) || (autonIndex >= 749 && !skills) || (Brain.timer(msec) - autonTimer >= 3000 && skills) || (autonIndex >= 2999 && skills)) {
        break;
      }
      autonIndex++;
    }

    std::cout << "brain timer: " << Brain.timer(msec) - autonTimer << std::endl;
    std::cout << "time stamp: " << timeStamp << std::endl;
    std::cout << "brain timer - time stamp: " << (Brain.timer(msec) - autonTimer) - timeStamp << std::endl;
    wait(20, msec);
    // std::cout << "\033[3A"; // go up 3 lines
    // std::cout << "\rarm: " << arm.value() << std::endl;
    // std::cout << "\033[1B"; // go down one line
    // std::cout << "\rclamp: " << clamp.value() << std::endl;
  }
  
  RUNDRIVETRAIN.stop();
  
  std::cout << "recording ended" << std::endl;

  if(recordAuton) {
    if(skills) {
      Brain.SDcard.savefile("skillsAxisData.bin", (uint8_t *)axisData, sizeof(axisData));
      Brain.SDcard.savefile("skillsClampData.bin", (uint8_t *)clampData, sizeof(clampData));
      Brain.SDcard.savefile("skillsArmData.bin", (uint8_t *)armData, sizeof(armData));
      Brain.SDcard.savefile("skillsIntakeData.bin", (uint8_t *)intakeData, sizeof(intakeData));
    }
    else if(side == POSITIVE) {
      Brain.SDcard.savefile("positiveAxisData.bin", (uint8_t *)axisData, sizeof(axisData));
      Brain.SDcard.savefile("positiveClampData.bin", (uint8_t *)clampData, sizeof(clampData));
      Brain.SDcard.savefile("positiveArmData.bin", (uint8_t *)armData, sizeof(armData));
      Brain.SDcard.savefile("positiveIntakeData.bin", (uint8_t *)intakeData, sizeof(intakeData));
    }
    else if(side == NEGATIVE) {
      Brain.SDcard.savefile("negativeAxisData.bin", (uint8_t *)axisData, sizeof(axisData));
      Brain.SDcard.savefile("negativeClampData.bin", (uint8_t *)clampData, sizeof(clampData));
      Brain.SDcard.savefile("negativeArmData.bin", (uint8_t *)armData, sizeof(armData));
      Brain.SDcard.savefile("negativeIntakeData.bin", (uint8_t *)intakeData, sizeof(intakeData));
    }
  }
  
  Brain.Screen.printAt(20, 80, "Auton Timer: %d", Brain.SDcard.size("controllerAxisValues.bin"));
  
  rightMotors.stop(brake);
  leftMotors.stop(brake);
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
