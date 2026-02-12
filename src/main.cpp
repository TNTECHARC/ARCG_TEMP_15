/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Organization:       Autonomous Robotics Club (ARC)                      */
/*    Authors:            Coby Smith and Joseph Dye                           */
/*    Created:            9/9/2024                                            */
/*    Description:        ARC Template                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "Auton.h"

using namespace vex;

////////////////////////// GLOBAL VARIABLES //////////////////////////

  // Competition Instance
  competition Competition;

  //Used for color sort
  // const int blueTeam = 1;
  // const int redTeam = 2;
  int teamColor = 1;
  

//////////////////////////////////////////////////////////////////////

///////////////////////// Prototypes /////////////////////////////////
void toggleDriveSpeed();
void SetSlot();
void TopSlotMajorityEnemy(int);
void transferArrayInfo();
void AutonSkills_Left();
void outTake();
void rotateRevolver();
void usercontrol();

bool armUp = false;
bool isInAuton = false;

//////////////////////////////////////////////////////////////////////

Drive chassis
(
    motor_group(L1, L2, L3, L4), // Left drive train motors 
    motor_group(R1, R2, R3, R4), // Right drive train motors
    PORT8,               // Inertial Sensor Port
    2.75,              // The diameter size of the wheel in inches
    1,                   // 
    12,                   // The maximum amount of the voltage used in the drivebase (1 - 12)
    NO_ODOM,
    1,                  //Odometry wheel diameter (set to zero if no odom) (1.96 robot behind by .2)
    0,               //Odom pod1 offset 
    0                //Odom pod2 offset
);

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
        2.0,       // Settle Error
        500,      // Time to Settle
        25000     // End Time
    );
    
}

/// @brief Runs before the competition starts
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

void toggleLift()
{
  liftL.set(!liftL.value());
  liftR.set(!liftR.value());
}

//Outtake function
void outTake() {
  if (!revolver.isSpinning()) 
  {
    armUp = true;
    outtake.stop(coast);
    outtake.setVelocity(100, percent);
    outtake.spinToPosition(160, degrees, true);
    outtake.spinFor(reverse, 0.75, sec);
    outtake.stop(hold);
    armUp = false;
  }
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
    bottomOuttake.spinToPosition(160, degrees, true);
    wait(0.1, sec);
    bottomOuttake.spinToPosition(0, degrees, true);
    armUp = false;
    isBottomOuttakeRunning = false;
  }
}

void moveIntake()
{
  if(!revolver.isSpinning())
  {
    intake.spin(forward, 12, volt);
  }
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


/******************************************************************
 * Function: FixGeneva()
 * Purpose: Reverse Rotate Geneva while button is pressed
*****************************************************************/
void FixGeneva() 
{
  if(!revolver.isSpinning())
    revolver.spin(reverse, 8, volt);
}


void moveSlot()
{
    outtake.setVelocity(100, percent);
    outtake.spinFor(reverse, 0.1, sec);
    outtake.stop(hold);
    revolver.setBrake(coast);

    PID revolverPID(0.66, 0, 2, 10, 100, 2000);
    float desiredPos = 360 + revolver.position(degrees);

    while(!revolverPID.isSettled())
    {
      Brain.Screen.setCursor(1,1);
      Brain.Screen.print("Is Revolving");
      float currentPos = revolver.position(degrees);

      float output = revolverPID.compute(desiredPos - currentPos);
      output = clamp(output, -12, 12);

      revolver.spin(forward, output, volt);
      wait(10, msec);
      Brain.Screen.clearScreen();
    }
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1,1);
    Brain.Screen.print("Is Revolved");

    revolver.spin(forward, 0, volt);
    revolver.setBrake(hold);
    outtake.stop(coast);
}


// Check Canister
bool isSlotFull();

/// @brief Runs during the UserControl section of the competition
void usercontrol() 
{
  isInAuton = true;

  R1.setBrake(coast);
  R2.setBrake(coast);
  R3.setBrake(coast);
  R4.setBrake(coast);
  L1.setBrake(coast);
  L2.setBrake(coast);
  L3.setBrake(coast);
  L4.setBrake(coast);

  extendo.set(true);

  outtake.spin(reverse, 9, volt);
  wait(0.1, sec);
  outtake.spin(reverse, 0, volt);
  outtake.stop(hold);

  Brain.Screen.clearScreen();

  //Team select function - note: this changes depending on the slot
  //teamColorSelect(teamColor); // Team selected

  backColorSensor.setLight(ledState::on);
  middleColorSensor.setLight(ledState::on);
  frontColorSensor.setLight(ledState::on);

  Controller1.ButtonR1.pressed(outTake);
  Controller1.ButtonR2.pressed(bottomOuttakeFunction);

  Controller1.ButtonL1.pressed(moveIntake);

  Controller1.ButtonLeft.pressed(FixGeneva);

  bool liftToggle = false;
  bool matchLoadToggle = false;


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
      if(!armUp) {
        moveSlot();
      }
    }

    //Automatic Rotation
    if((Controller1.ButtonL1.pressing() || Controller1.ButtonL2.pressing()) && isSlotFull())
      {
        if (!armUp) {
        moveSlot();
      }
     }

    // Rise / Fall Toggle
    
    if (Controller1.ButtonRight.pressing() && !liftToggle) {
      toggleLift();
      liftToggle = true;
    }
    else if(!Controller1.ButtonRight.pressing() && liftToggle)
    {
      liftToggle = false;
    }

    if(Controller1.ButtonL2.pressing() && !matchLoadToggle)
    {
      matchLoader.set(!matchLoader.value());
      matchLoadToggle = true;
    }
    else if(!Controller1.ButtonL2.pressing() && matchLoadToggle)
    {
      matchLoadToggle = false;
    }



    if(!Controller1.ButtonL1.pressing() && !Controller1.ButtonL2.pressing() && !Controller1.ButtonR2.pressing())
    {
      intake.spin(reverse, 0, volt);
    }

    chassis.arcade();
    wait(20, msec); // Sleep the task for a short amount of time to
    //Brain.Screen.clearScreen();
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