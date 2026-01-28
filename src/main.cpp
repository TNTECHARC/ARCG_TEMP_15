/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Organization:       Autonomous Robotics Club (ARC)                      */
/*    Authors:            Coby Smith and Joesph Dye                           */
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

  bool isInAuton = false;
  int lastPressed = 0;

  bool armUp = false;

  Drive chassis
  (
    motor_group(L1, L2, L3, L4), // Left drive train motors
    motor_group(R1, R2, R3, R4), // Right drive train motors
    PORT6,               // Inertial Sensor Port
    2.75,              // The diameter size of the wheel in inches
    1,                   // 
    12,                   // The maximum amount of the voltage used in the drivebase (1 - 12)
    odomType,
    1,                  //Odometry wheel diameter (set to zero if no odom) (1.96 robot behind by .2)
    0,               //Odom pod1 offset 
    0                //Odom pod2 offset
  );

//////////////////////////////////////////////////////////////////////

///////////////////////// Prototypes /////////////////////////////////
void setDriveTrainConstants();
void toggleDriveSpeed();
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
  chassis.setPosition(0,0,0);
  setDriveTrainConstants();
  Auton_Right1();
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

int revolverSlots [6][3];
int currentSlot = 0;

//Rotate revolver
void moveSlot()
{
  revolver.setTimeout(0.5, seconds);
  revolver.setVelocity(100, percent);
  revolver.spinFor(1, rev);

}

//Outtake function
void outTake() {
  if(!revolver.isSpinning())
  {
    armUp = true;
    outtakeLeft.setVelocity(60, percent);
    outtakeRight.setVelocity(60, percent);
    outtakeLeft.spinToPosition(110, degrees, true);
    outtakeRight.spinToPosition(110, degrees, true);
    outtakeLeft.spin(reverse, 10, volt);
    outtakeRight.spin(reverse, 10, volt);
    wait(0.4, sec);
    outtakeLeft.stop(hold);
    outtakeRight.stop(hold);
    armUp = false;
    moveSlot();

    // Reset Slot 
    int tempSlotNumber = currentSlot + 3;
    if(tempSlotNumber > 6 )
      tempSlotNumber = tempSlotNumber - 6;
    
    revolverSlots[tempSlotNumber][0] = 0;
    revolverSlots[tempSlotNumber][1] = 0;
    revolverSlots[tempSlotNumber][2] = 0;

    currentSlot++;
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
    bottomOuttake.spinToPosition(200, degrees, true);
    bottomOuttake.spin(reverse, 12, volt);
    wait(0.8, sec);
    bottomOuttake.stop(hold);
    //intakeLeft.spin(reverse, 0, volt);
    //intakeRight.spin(reverse, 0, volt);
    armUp = false;
    isBottomOuttakeRunning = false;

    revolverSlots[currentSlot][0] = 0;
    revolverSlots[currentSlot][1] = 0;
    revolverSlots[currentSlot][2] = 0;
  }
}

//Rise!!
void rise() {

  liftR.set(true);
  wait(10, msec);
  liftL.set(true);
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

// Function to Unload Specific Color --[MUST TEST]--
void unload(color c) {
  for (int i = 0; i < 6; i++) {
    if (revolverSlots[i][0] == c || revolverSlots[i][1] == c || revolverSlots[i][2] == c) {
      while (currentSlot != i) {
        moveSlot();
        waitUntil(!revolver.isSpinning());
      }

      outTake();
    }
  }
}

// Check Canister
bool isSlotFull()
{
  if((frontColorSensor.hue() <= 20 && frontColorSensor.hue() >= 0) ||
    (frontColorSensor.hue() <= 170 && frontColorSensor.hue() >= 200) &&
    (middleColorSensor.hue() <= 20 && middleColorSensor.hue() >= 0) ||
    (middleColorSensor.hue() <= 170 && middleColorSensor.hue() >= 200) &&
    (backColorSensor.hue() <= 20 && backColorSensor.hue() >= 0) || 
    (backColorSensor.hue() <= 170 && backColorSensor.hue() >= 200)
    )
    {
      Brain.Screen.setCursor(1,1);
      Brain.Screen.print("Is Full");
      return true;
    }
  else 
    return false;
}

// Check Revolver
bool isCurrentSlotFilled()
{
  if(revolverSlots[currentSlot][0] != 0 || revolverSlots[currentSlot][1] != 0 || revolverSlots[currentSlot][2] != 0)
    return true;
  else
    return false;
}

void changeSlot()
{
  // SetSlotColor 0
  if(backColorSensor.color() == red)
    revolverSlots[currentSlot][0] = 2;
  else
    revolverSlots[currentSlot][0] = 1;
  
  // SetSlotColor 1

  if(middleColorSensor.color() == red)
    revolverSlots[currentSlot][0] = 2;
  else
    revolverSlots[currentSlot][0] = 1;

  // SetSlotColor 2

  if(frontColorSensor.color() == red)
    revolverSlots[currentSlot][0] = 2;
  else
    revolverSlots[currentSlot][0] = 1;

  // Change Current Slot
  if(currentSlot == 5)
    currentSlot = 0;
  else
    currentSlot++;
  
  wait(300, msec);
  moveSlot();
}

void moveIntake()
{
  if(!revolver.isSpinning())
  {
    intake.spin(forward, 12, volt);
  }
}

void intakeMoveSlot()
{
  
}

/// @brief Runs during the UserControl section of the competition
void usercontrol() 
{
  isInAuton = true;
  Brain.Screen.clearScreen();
  bool isSpinning = false;

  Controller1.ButtonUp.pressed(rise);
  Controller1.ButtonDown.pressed(fall);
  backColorSensor.setLight(ledState::on);
  middleColorSensor.setLight(ledState::on);
  frontColorSensor.setLight(ledState::on);

  Controller1.ButtonR1.pressed(outTake);
  Controller1.ButtonR2.pressed(bottomOuttakeFunction);

  Controller1.ButtonL1.pressed(moveIntake);
  Controller1.ButtonL2.pressed(moveIntake);
  


  // User control code here, inside the loop
  while (1) 
  {

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

    // if(Controller1.ButtonR1.pressing() && !revolver.isSpinning())
    // {
    //   thread outtakeThread = thread(outTake);
    // }

    // if(Controller1.ButtonR2.pressing() && !revolver.isSpinning())
    // {
    //   thread outtakeThread = thread(bottomOuttakeFunction);
    // }

    // if(Controller1.ButtonL2.pressing())
    // {
    //   thread unloadThread = thread(unloadAll);
    // }

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



/// @brief Sets the PID values for the DriveTrain
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
        0.504,    // Kp - Proportion Constant
        0.0,      // Ki - Integral Constant
        4.05,      // Kd - Derivative Constant 
        1.0,    // Settle Error
        500,    // Time to Settle
        2500    // End Time
    );
    
}

/// @brief Auton Right Slot 1 [BLUE] - Write code for route within this function.
void Auton_Right1() {
    Brain.Screen.print("EXECUTING: Auton 1 - RIGHT");
    
    /*
        -- Modify:: drive/turn functions (include: minVoltage, precedence)
        -- Add:: Additional tests and hardware as needed
    */

    // Initial Diagnostics
    std::cout << std::endl << std::endl << std::endl;
    std::cout << "_______________________________" << std::endl;
    std::cout << std::setw(27) << "EXECUTING: Auton Right" << std::endl;
    std::cout << "Starting Position:  " << chassis.getCurrentMotorPosition() << std::endl;
    std::cout << "Starting Heading:   " << inertial1.heading() << std::endl;

    // Drive from Origin to Loader (Right)
    chassis.driveDistance(34, 3.0, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.25, sec);
    chassis.turnToAngle(90, 3.0, 9.0, false);
    std::cout << inertial1.heading() << std::endl;
    wait(0.25, sec);
    moveIntake();

    // Load Blocks from Loader (including: Extra given Loader Blocks [6])
    chassis.driveDistance(11, 3.0, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(2, sec); // Adjust time as needed for optimal loading

    // Reverse to Load Side Blocks [2]
    chassis.driveDistance(-12, 3.0, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.25, sec);
    chassis.turnToAngle(0, 3.0, 9.0, false);
    std::cout << inertial1.heading() << std::endl;
    wait(0.25, sec);
    chassis.driveDistance(12.5, 3.0, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(1.5, sec);

    // Drive to Center/Park Zone to EXCHANGE Blocks ??
    chassis.driveDistance(-52, 3.0, 12.0, false);
    wait(0.25, sec);
    chassis.turn(180, 9.0);
    std::cout << inertial1.heading() << std::endl;

    // EXCHANGE Blocks with Other Robot
    chassis.brake();
    unload(blue);
    
    // Load Alliance Blocks
    // Score Alliance Blocks (either in BOTH Long && ONE Center Goals, OR ONE Long && BOTH Center Goals)
}


/// @brief Auton Right Slot 2 [RED] - Write code for route within this function.
void Auton_Right2() {
    Brain.Screen.print("EXECUTING: Auton 2 - RIGHT");
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
void Auton_Left1() {
    Brain.Screen.print("EXECUTING: Auton 1 - RIGHT");
    
    /*
        -- Fix:: Settle Time && Voltage (drive/turn stoppage and step timing)
        -- Modify:: drive/turn functions (include: minVoltage, precedence)
        -- Add:: Additional tests and hardware as needed (finalize 24" and 15" ASAP)
    */
    chassis.driveDistance(38, 3.0, 12.0, false);
    chassis.turnToAngle(-90, 3.0, 12.0, false);
    chassis.driveDistance(18, 3.0, 12.0, false);
    moveIntake();
    wait(5, sec);
    chassis.driveDistance(-16, 3.0, 12.0, false);
    chassis.turnToAngle(0, 3.0, 12.0, false);
    chassis.driveDistance(26, 3.0, 12.0, false);
    chassis.turnToAngle(170, 3.0, 12.0, false);
    chassis.driveDistance(48, 3.0, 12.0, false);
    chassis.brake();
    unload(red);
    // Load Blue Blocks
    // Grab Blue Park Zone Blocks
    // Park
}


/// @brief Auton Left Slot 2 [RED] - Write code for route within this function.
void Auton_Left2() {
    Brain.Screen.print("EXECUTING: Auton 2 - LEFT");
}

/// @brief Auton Left Slot 3 [BLUE] - Write code for route within this function.
void Auton_Left3() {
    Brain.Screen.print("EXECUTING: Auton 3 - LEFT");
}

/// @brief Auton Left Slot 4 [RED] - Write code for route within this function.
void Auton_Left4() {
    Brain.Screen.print("EXECUTING: Auton 4 - LEFT");
}