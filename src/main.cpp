/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Organization:       Autonomous Robotics Club (ARC)                      */
/*    Authors:            Coby Smith and Joseph Dye                           */
/*    Created:            9/9/2024                                            */
/*    Description:        ARC Template                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include "screen.h"
#include "util.h"
#include "Drive.h"
#include "PID.h"

using namespace vex;

////////////////////////// GLOBAL VARIABLES //////////////////////////

  // Competition Instance
  competition Competition;

  int odomType = NO_ODOM;

  float velocity = 12.0;
  bool boost = true;

  bool isInAuton = false;
  int lastPressed = 0;

  bool armUp = false;

  //Used for color sort
  const int blueTeam = 1;
  const int redTeam = 2;
  int teamColor = 1;


  Drive chassis
  (
    motor_group(L1, L2, L3, L4), // Left drive train motors 
    motor_group(R1, R2, R3, R4), // Right drive train motors
    PORT8,               // Inertial Sensor Port
    2.75,              // The diameter size of the wheel in inches
    1,                   // 
    velocity,                   // The maximum amount of the voltage used in the drivebase (1 - 12)
    odomType,
    1,                  //Odometry wheel diameter (set to zero if no odom) (1.96 robot behind by .2)
    0,               //Odom pod1 offset 
    0                //Odom pod2 offset
  );

//////////////////////////////////////////////////////////////////////

///////////////////////// Prototypes /////////////////////////////////
void setDriveTrainConstants();
void toggleDriveSpeed();
void SetSlot();
bool TopSlotMajorityEnemy(int);
void transferArrayInfo();
void Auton_Right1();
void Auton_Right2();
void Auton_Right3();
void Auton_Right4();
void Auton_Left1();
void Auton_Left2();
void Auton_Left3();
void Auton_Left4();

//////////////////////////////////////////////////////////////////////


/// @brief Runs before the competition starts
void preAuton() 
{
  setDriveTrainConstants();
  enum preAutonStates{START_SCREEN = 0, SELECTION_SCREEN = 1};
  int currentScreen = START_SCREEN;
  int lastPressed = 0;

  // Calibrates/Resets the Brains sensors before the competition
  inertial1.calibrate();
  rotation1.resetPosition();
  rotation2.resetPosition();

  vex::color colors[8] = {vex::color::red, vex::color::red, vex::color::red, vex::color::red, 
                          vex::color::blue, vex::color::blue, vex::color::blue, vex::color::blue};
  std::string names[8] = {"Auton L1", "Auton R1", "Auton L3", "Auton R3", 
                          "Auton L2", "Auton R2", "Auton L4", "Auton R4"};
  Button buttons[9];
  createAutonButtons(colors, names, buttons);
  buttons[0].setChosen(true);

  Text selectionLabel;
  Button selectionButton;
  createPreAutonScreen(selectionButton, selectionLabel);
  
  //int lastPressed = 0;
  int temp;

  Controller1.Screen.print(buttons[lastPressed].getName().c_str());

  while(!isInAuton){
    showPreAutonScreen(selectionButton, selectionLabel, buttons[lastPressed].getName());
    while(currentScreen == START_SCREEN){
      if(Brain.Screen.pressing()){
        if(checkPreAutonButton(selectionButton)){
          currentScreen = SELECTION_SCREEN;
        }
      }
      wait(10, msec);
    }

    showAutonSelectionScreen(buttons);
    while(currentScreen == SELECTION_SCREEN){
      if(Brain.Screen.pressing()){
        temp = checkButtonsPress(buttons);
        if(temp >= 0 && temp < 8){
          lastPressed = temp;
          Controller1.Screen.clearLine();
          Controller1.Screen.setCursor(1, 1);
          Controller1.Screen.print(buttons[lastPressed].getName().c_str());
        }
      }
      if(temp == 8)
        currentScreen = START_SCREEN;
      wait(10, msec);
    }
    wait(10, msec);
  }
  Brain.Screen.clearScreen();
}

/// @brief Runs during the Autonomous Section of the Competition
void autonomous() 
{
  isInAuton = true;

  extendo.set(true);

  chassis.setPosition(0,0,0);
  setDriveTrainConstants();

  /* Add switch for input button mapping
  
  OR link specific robots to Autons

  // switch (lastPressed) 
  // {
  //   case 1:
  //     Auton_1();
  //     break;
  //   case 2:
  //     Auton_2();
  //     break;
  //   case 3:
  //     Auton_3;
  //     break;
  //   case 4:
  //     Auton_4();
  //     break;
  //   case 5:
  //     Auton_5();
  //     break;
  //   case 6:
  //     Auton_6();
  //     break;
  //   case 7:
  //     Auton_7();
  //     break;
  //   case 8:
  //     Auton_8();
  //     break;
  //   default:
  //     DefaultAuton();
  //     break;
  // } */
}

// 0 - Empty
// 1 - Blue
// 2 - Red

// int revolverSlots [6][3];
// int SlotNum = 0;

// Color Sort (Blue team = 1, Red Team = 2)
// void teamColorSelect(int teamColor) 
// {
//   if(teamColor == 1) //Blue Team
//   {
//     Controller1.Screen.clearScreen();
//     Controller1.Screen.setCursor(0,0);
//     Controller1.Screen.print("Blue Team Selected");
//   }
//   if(teamColor == 2) //Red Team
//   {
//     Controller1.Screen.clearScreen();
//     Controller1.Screen.setCursor(0,0);
//     Controller1.Screen.print("Red Team Selected");
//   }

// }



//Rotate revolver
void moveSlot()
{
  //SetSlot(); //Sets the colors in the 2D array for each slot
  //transferArrayInfo();
  revolver.setTimeout(0.5, seconds);
  revolver.setVelocity(100, percent);
  revolver.spinFor(1, rev);
}

//Outtake function   ********HAVE SOMEONE LOOK AT HOW COLOR SORT MODIFIED THIS*********
void outTake() {
 

  // float pidCompute;

  // PID outtakePID(50, // Proportion
  //     0.5,            // Integral
  //     0.01,          // Derivative
  //     0.1,          // Settle Error
  //     100,         // Time to Settle (ms)
  //     2000);      // End Time (ms)

  // if(!revolver.isSpinning())
  // {
  //   armUp = true;

  //   while (!outtakePID.isSettled()) { // Desired: 50 degrees

  //     pidCompute = outtakePID.compute(50.0 - outtake.position(degrees));
  //     pidCompute = clamp(pidCompute, -12.0, 12.0);

  //     if ((pidCompute > 0) && (pidCompute < 1.0)) {
  //       pidCompute = 1.0;
  //     } else if (pidCompute < 0 && pidCompute > -1.0) {
  //       pidCompute = -1.0;
  //     }

  //     outtake.spin(forward, pidCompute, volt);
  //     wait(1, sec);
  //     outtake.spinToPosition(0, degrees, true);
  //     outtake.stop(hold);
  //     moveSlot();
  //   }
  if((!revolver.isSpinning())) //&& (!TopSlotMajorityEnemy(teamColor)))
  {
    armUp = true;
    outtake.setVelocity(100, percent);
    outtake.spinToPosition(80, degrees, true);
    outtake.spinToPosition(0, degrees, true);
    wait(0.2,seconds);
    outtake.stop(hold);
    armUp = false;
    moveSlot();

  // Reset Slot 
    // revolverSlots[3][0] = 0;
    // revolverSlots[3][1] = 0;
    // revolverSlots[3][2] = 0;

  }
  // else {
  //   if(TopSlotMajorityEnemy(teamColor)) 
  //   {
  //     moveSlot();
  //   }
  //  }
}

bool isBottomOuttakeRunning = false;
void bottomOuttakeFunction()
{
  if(!revolver.isSpinning())
  {
    isBottomOuttakeRunning = true;
    armUp = true;
    intake.spin(reverse, 12, volt);
    bottomOuttake.setVelocity(100, percent);
    bottomOuttake.spinToPosition(200, degrees, true);
    bottomOuttake.spin(reverse, 12, volt);
    wait(0.8, sec);
    bottomOuttake.stop(hold);
    //intakeLeft.spin(reverse, 0, volt);
    //intakeRight.spin(reverse, 0, volt);
    armUp = false;
    isBottomOuttakeRunning = false;

    // revolverSlots[0][0] = 0;
    // revolverSlots[0][1] = 0;
    // revolverSlots[0][2] = 0;
  }
}

//Rise!!
void rise() {

  liftL.set(true);
  wait(10, msec);
  liftR.set(true);
}

//Fall!
void fall() {

  liftL.set(false);
  liftR.set(false);
}

//function to unload all
void unloadAll() {
  for(int i = 0; i <6; i++)
      {
        outTake();
        waitUntil(!revolver.isSpinning());
      }
}


// Check Canister
bool isSlotFull()
{
  //Says, "If frontSensor is between 0 and 20(red), or frontSensor is between 170 and 200(blue)"
  //AND "MiddleSensor is red or blue"
  //AND "BackSensor is red or blue"
  //Then return that Slot is full (true)
  if((((frontColorSensor.hue() <= 20 && frontColorSensor.hue() >= 0)) ||
    ((frontColorSensor.hue() <= 170 && frontColorSensor.hue() >= 200))) &&
    (((middleColorSensor.hue() <= 20 && middleColorSensor.hue() >= 0)) ||
    ((middleColorSensor.hue() <= 170 && middleColorSensor.hue() >= 200))) &&
    (((backColorSensor.hue() <= 20 && backColorSensor.hue() >= 0)) || 
    ((backColorSensor.hue() <= 170 && backColorSensor.hue() >= 200))))
    {
      Brain.Screen.setCursor(1,1);
      Brain.Screen.print("Is Full");
      return true;
    }
  else 
    return false;
}


// // Check Revolver
// bool isBottomSlotFilled()
// {
//   if(revolverSlots[0][0] != 0 || revolverSlots[0][1] != 0 || revolverSlots[0][2] != 0)
//     return true;
//   else
//     return false;
// }



void moveIntake()
{
  if(!revolver.isSpinning())
  {
    intake.spin(forward, 12, volt);
  }
}


/******************************************************************
 * Function: FixGeneva()
 * Purpose: Reverse Rotate Geneva while button is pressed
*****************************************************************/
void FixGeneva() 
{
  if(!revolver.isSpinning())
    revolver.spin(reverse, 8, volt);
}
//******************************************************************************/
//COLOR SORTING EVERYTHING!!!!!!!!!!!!

// void transferArrayInfo() 
// {

//   int tempArr[6][3] = {
//         {0, 0, 0},
//         {0, 0, 0},
//         {0, 0, 0},
//         {0, 0, 0},
//         {0, 0, 0},
//         {0, 0, 0}
//     };

//   //Shifts everything in revolverSlots by 1 in tempArr
//   for(int i = 0; i < 6; i++)
//   {
//     for(int j = 0; j < 3; j++)
//     {
//       if(i == 5)
//       {
//         tempArr[0][j] = revolverSlots[i][j];
//       }
//       else
//         tempArr[i+1][j] = revolverSlots[i][j];
//     }

//     //Puts shifted temp into revolverSlots to complete the transfer
//     for(int i = 0; i < 6; i++) 
//     {
//       for(int j = 0; j < 3; j++) 
//       {
//         revolverSlots[i][j] = tempArr[i][j];
//       }
//     }
//   }

// }


/******************************************************************
 * Function: SetSlot()
 * Purpose: Assign 0, 1, or 2 to each sensor within slots 0-5
*****************************************************************/

// void SetSlot()
// {
//   // SetSlotColor 0
//   if((backColorSensor.hue() <= 20) && (backColorSensor.hue() >= 0))
//     revolverSlots[0][0] = 2;
//   else if((backColorSensor.hue() <= 170) && (backColorSensor.hue() >= 200))
//     revolverSlots[0][0] = 1;
//   else
//     revolverSlots[0][0] = 0;
  
//   // SetSlotColor 1                        

//   if((middleColorSensor.hue() <= 20) && (middleColorSensor.hue() >= 0))
//     revolverSlots[0][1] = 2;
//   else if((middleColorSensor.hue() <= 170) && (middleColorSensor.hue() >= 200))
//     revolverSlots[0][1] = 1;
//   else
//     revolverSlots[0][1] = 0;

//   // SetSlotColor 2

//   if((frontColorSensor.hue() <= 20) && (frontColorSensor.hue() >= 0))
//     revolverSlots[0][2] = 2;
//   else if((frontColorSensor.hue() <= 170) && (frontColorSensor.hue() >= 200))
//     revolverSlots[0][2] = 1;
//   else
//     revolverSlots[0][2] = 0;
  
//   wait(300, msec);
  
// }

/******************************************************************
 * Function: TopSlotMajorityEnemy()
 * Purpose: Check if the top Slot's majority is the enemy color
*****************************************************************/
// bool TopSlotMajorityEnemy(int teamColor) 
// {
//   if(((revolverSlots[3][0] == teamColor) && (revolverSlots[3][1] == teamColor))  || //Finds if majority is our team!
//       ((revolverSlots[3][1] == teamColor) && (revolverSlots[3][2] == teamColor)) ||
//       ((revolverSlots[3][0] == teamColor) && (revolverSlots[3][2] == teamColor)))
//     return false;

//   else //If majority is not our team, it is the enemy team!
//     return true;
// }

/*************************************************************************************/

void setBoost() {
  if (!boost) {
    velocity = 12.0;
    boost = true;
  } else if (boost) {
    velocity = 1.0;
    boost = false;
  }
}

/// @brief Runs during the UserControl section of the competition
void usercontrol() 
{
  isInAuton = true;
  extendo.set(true);

  Brain.Screen.clearScreen();
  bool isSpinning = false;

  //Team select function - note: this changes depending on the slot
  //teamColorSelect(teamColor); // Team selected

  Controller1.ButtonUp.pressed(rise);
  Controller1.ButtonDown.pressed(fall);
  backColorSensor.setLight(ledState::on);
  middleColorSensor.setLight(ledState::on);
  frontColorSensor.setLight(ledState::on);

  Controller1.ButtonR1.pressed(outTake);
  Controller1.ButtonR2.pressed(bottomOuttakeFunction);

  Controller1.ButtonL1.pressed(moveIntake);
  Controller1.ButtonL2.pressed(moveIntake);

  Controller1.ButtonLeft.pressed(FixGeneva);


  Controller1.ButtonX.pressed(setBoost);

  // User control code here, inside the loop
  while (1) 
  {

    //To stop geneva fixer from spinning when button is released
    if(!Controller1.ButtonLeft.pressing())
    {
      revolver.spin(forward, 0, volt);
    }

    if(Controller1.ButtonB.pressing() && !revolver.isSpinning())
    {
      if(armUp == false) {
        moveSlot();
      }
    }

    // if((Controller1.ButtonL1.pressing() || Controller1.ButtonL2.pressing()) && isSlotFull())
    //   {
    //     if(armUp == false) {
    //     moveSlot();
    //   }
     // }

  

    //Automatic Rotation
    if((Controller1.ButtonL1.pressing() || Controller1.ButtonL2.pressing()) && isSlotFull())
      {
        if(armUp == false) {
        moveSlot();
      }
     }



     //
    if(Controller1.ButtonL2.pressing() && !revolver.isSpinning())
    {
      matchLoader.set(true);
    }else
    {
      matchLoader.set(false);
      if(!Controller1.ButtonL1.pressing() && !Controller1.ButtonR2.pressing())
      {
        intake.spin(reverse, 0, volt);
      }
    }

    chassis.arcade();
    wait(20, msec); // Sleep the task for a short amount of time to
    Brain.Screen.clearScreen();
  }

}


int main() 
{

  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  preAuton();

  // Prevent main from exiting with an infinite loop.
  while (true) 
  {
    wait(100, msec);
  }
}



/// @brief Sets the PID values for the Chassis
void setDriveTrainConstants()
{
    // Set the Drive PID values for the DriveTrain
    chassis.setDriveConstants(
        2.481, // Kp - Proportion Constant
        0.0, // Ki - Integral Constant
        10.0, // Kd - Derivative Constant
        1.0, // Settle Error
        100, // Time to Settle
        25000 // End Time
    );

    // Set the Turn PID values for the DriveTrain
    chassis.setTurnConstants(
        0.504,        // Kp - Proportion Constant
        0.0,         // Ki - Integral Constant
        4.05,       // Kd - Derivative Constant 
        1.0,       // Settle Error
        500,      // Time to Settle
        25000     // End Time
    );
    
}

/// @brief Auton Right Slot 1 [BLUE] - Write code for route within this function.
void Auton_Right1() { // Strategy: Score (Right)
    Brain.Screen.print("EXECUTING: Auton 1 - RIGHT");

// Initial Diagnostics
    std::cout << std::endl << std::endl << std::endl;
    std::cout << "_______________________________" << std::endl;
    std::cout << std::setw(27) << "EXECUTING: Auton 1 - Right" << std::endl;
    std::cout << "Starting Position:  " << chassis.getCurrentMotorPosition() << std::endl;
    std::cout << "Starting Heading:   " << inertial1.heading() << std::endl;

// Drive from Origin to LOADER (Right)
    // chassis.driveDistance(-2, 3.0, 12.0, false);
    // chassis.turnToAngle(-90, 3.0, 9.0, false);
    std::cout << inertial1.heading() << std::endl;
    chassis.driveDistance(34, 3.0, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.25, sec);
    chassis.turn(90, 9.0);
    std::cout << inertial1.heading() << std::endl;
    wait(0.25, sec);
    moveIntake();

// Load Blocks from Loader (including: Extra given Loader Blocks [6])
    // Not loading reliably, needs tuning && other systems
    chassis.driveDistance(12.5, 3.0, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(2, sec); // Adjust time as needed for optimal loading

// Reverse to Load Side Blocks [2]
    chassis.driveDistance(-12, 3.0, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.25, sec);
    chassis.turn(-90, 9.0);
    std::cout << inertial1.heading() << std::endl;
    wait(0.25, sec);
    // Loads BOTH Side Blocks consistently && reliably
    chassis.driveDistance(12.5, 3.0, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(1.5, sec);

// Drive to RIGHT Long Goal to SCORE
    // Needs to be tested, tuned, finalized
    chassis.driveDistance(-15, 3.0, 12.0, false);
    wait(0.25, sec);
    chassis.turn(-90, 9.0);
    std::cout << inertial1.heading() << std::endl;
    chassis.driveDistance(22, 3.0, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    rise();
    wait(0.5, sec);
    outTake();
    std::cout << "Scoring in Left Long Goal" << std::endl;
    wait(3, sec);

// Drive to Center Goal
    fall();
    chassis.driveDistance(-5, 3.0, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    chassis.turn(-90, 9.0);
    std::cout << inertial1.heading() << std::endl;
    chassis.driveDistance(24, 3.0, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    chassis.turn(45, 9.0);
    std::cout << inertial1.heading() << std::endl;
    chassis.driveDistance(10, 3.0, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    outTake();
    wait(5, sec);


    chassis.brake();
}


/// @brief Auton Right Slot 2 [RED] - Write code for route within this function.
void Auton_Right2() { // Strategy: Center Horde
    Brain.Screen.print("EXECUTING: Auton 2 - RIGHT");

// Initial Diagnostics
    std::cout << std::endl << std::endl << std::endl;
    std::cout << "_______________________________" << std::endl;
    std::cout << std::setw(27) << "EXECUTING: Auton Right - 2" << std::endl;
    std::cout << "Starting Position:  " << chassis.getCurrentMotorPosition() << std::endl;
    std::cout << "Starting Heading:   " << inertial1.heading() << std::endl;

// Drive from Origin to SIDE BLOCKS (Right)
    chassis.driveDistance(48, 3.0, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.25, sec);


// Load Side Blocks [2]
    moveIntake();
    wait(2, sec);

// Drive to Center for Blocks [6, 3 of each color]
    // NOT Auton Line Blocks [4, 2 of each color] >> Get during Driver Control
    // Needs to be tested, tuned, finalized
    chassis.driveDistance(-42, 3.0, 12.0, false);
    wait(0.25, sec);
    chassis.turn(-90, 9.0);
    std::cout << inertial1.heading() << std::endl;
    chassis.driveDistance(48, 3.0, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    moveIntake();
    chassis.turn(90, 9.0);
    std::cout << inertial1.heading() << std::endl;
    chassis.driveDistance(12, 3.0, 9.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;

// Return to Safe Position
    wait(0.5, sec);
    chassis.driveDistance(-6, 3.0, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    chassis.turn(-90, 9.0);
    std::cout << inertial1.heading() << std::endl;
    chassis.driveDistance(-24, 3.0, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    std::cout << "Horded Successfully" << std::endl;


    chassis.brake();
    // Horde Opponent Blocks
}

/// @brief Auton Right Slot 3 [BLUE] - Write code for route within this function.
void Auton_Right3() {
    Brain.Screen.print("EXECUTING: Auton 3 - RIGHT");
}

/// @brief Auton Right Slot 4 [RED]- Write code for route within this function.
void Auton_Right4() {
    Brain.Screen.print("EXECUTING: Auton 4 - RIGHT");
}

/// @brief Auton Left Slot 1 [BLUE]- Write code for route within this function.
void Auton_Left1() { // Strategy: Score (Left)
    Brain.Screen.print("EXECUTING: Auton 1 - LEFT");

    // Initial Diagnostics
    std::cout << std::endl << std::endl << std::endl;
    std::cout << "_______________________________" << std::endl;
    std::cout << std::setw(27) << "EXECUTING: Auton 1 - Left" << std::endl;
    std::cout << "Starting Position:  " << chassis.getCurrentMotorPosition() << std::endl;
    std::cout << "Starting Heading:   " << inertial1.heading() << std::endl;

// Drive from Origin to LOADER (Left)
    // chassis.driveDistance(-2, 3.0, 12.0, false);
    // chassis.turnToAngle(90, 3.0, 9.0, false);
    chassis.driveDistance(34, 3.0, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.25, sec);
    chassis.turn(-90, 9.0);
    std::cout << inertial1.heading() << std::endl;
    wait(0.25, sec);
    moveIntake();

// Load Blocks from Loader (including: Extra given Loader Blocks [6])
    // Not loading reliably, needs tuning && other systems
    chassis.driveDistance(12.5, 3.0, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(2, sec); // Adjust time as needed for optimal loading

// Reverse to Load Side Blocks [2]
    chassis.driveDistance(-12, 3.0, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.25, sec);
    chassis.turn(90, 9.0);
    std::cout << inertial1.heading() << std::endl;
    wait(0.25, sec);
    // Loads BOTH Side Blocks consistently && reliably
    chassis.driveDistance(12.5, 3.0, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(1.5, sec);

// Drive to LEFT Long Goal to SCORE
    // Needs to be tested, tuned, finalized
    chassis.driveDistance(-15, 3.0, 12.0, false);
    wait(0.25, sec);
    chassis.turn(90, 9.0);
    std::cout << inertial1.heading() << std::endl;
    chassis.driveDistance(22, 3.0, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    rise();
    wait(0.5, sec);
    outTake();
    std::cout << "Scoring in Left Long Goal" << std::endl;
    wait(3, sec);
    fall();

// Drive to Park Zone & Park
    // chassis.driveDistance(-5, 3.0, 12.0, false);
    // chassis.turn(145, 9.0);
    // chassis.driveDistance(45, 3.0, 12.0, false);


    chassis.brake();
    
    // For AUTON WIN POINT
}


/// @brief Auton Left Slot 2 [RED] - Write code for route within this function.
void Auton_Left2() { // Strategy: Center Horde
    Brain.Screen.print("EXECUTING: Auton 2 - RIGHT");

// Initial Diagnostics
    std::cout << std::endl << std::endl << std::endl;
    std::cout << "_______________________________" << std::endl;
    std::cout << std::setw(27) << "EXECUTING: Auton Right - 2" << std::endl;
    std::cout << "Starting Position:  " << chassis.getCurrentMotorPosition() << std::endl;
    std::cout << "Starting Heading:   " << inertial1.heading() << std::endl;

// Drive from Origin to SIDE BLOCKS (Right)
    chassis.driveDistance(48, 3.0, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.25, sec);


// Load Side Blocks [2]
    moveIntake();
    wait(2, sec);

// Drive to Center for Blocks [6, 3 of each color]
    // NOT Auton Line Blocks [4, 2 of each color] >> Get during Driver Control
    // Needs to be tested, tuned, finalized
    chassis.driveDistance(-42, 3.0, 12.0, false);
    wait(0.25, sec);
    chassis.turn(90, 9.0);
    std::cout << inertial1.heading() << std::endl;
    chassis.driveDistance(48, 3.0, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    chassis.turn(-90, 9.0);
    std::cout << inertial1.heading() << std::endl;
    moveIntake();
    chassis.driveDistance(12, 3.0, 9.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;

// Return to Safe Position
    wait(0.5, sec);
    chassis.driveDistance(-6, 3.0, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    chassis.turn(90, 9.0);
    std::cout << inertial1.heading() << std::endl;
    chassis.driveDistance(-24, 3.0, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;

    
    std::cout << "Horded Successfully" << std::endl;
    std::cout << "Chassis Protected" << std::endl;
    chassis.brake();
    // Horde Opponent Blocks
}

/// @brief Auton Left Slot 3 [BLUE] - Write code for route within this function.
void Auton_Left3() {
    Brain.Screen.print("EXECUTING: Auton 3 - LEFT");
}

/// @brief Auton Left Slot 4 [RED] - Write code for route within this function.
void Auton_Left4() {
    Brain.Screen.print("EXECUTING: Auton 4 - LEFT");
}