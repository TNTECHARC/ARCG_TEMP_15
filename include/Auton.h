#pragma once

#include "vex.h"
#include "screen.h"
#include "util.h"
#include "Drive.h"
#include "PID.h"

using namespace vex;

void setDriveTrainConstants();
void usercontrol();
void preAuton();
void Auton_Right1();
void Auton_Right2();
void Auton_Right3();
void Auton_Left1();
void Auton_Left2();
void Auton_Left3();
void AutonSkills_Right();
void AutonSkills_Left();
void moveIntake();
void outTake();
void moveSlot();
void rise();
void fall();
void autonomous();
void rotateRevolver();

extern Drive chassis;

extern bool isInAuton;
extern float minVoltage;
extern bool armUp;
extern bool isBottomOuttakeRunning;

