#include "Auton.h"

/*************************************************************************************/

/// @brief Runs during the Autonomous Section of the Competition
void autonomous() 
{
    isInAuton = true;

    rise();
    extendo.set(true);
    wait(0.25, sec);
    fall();
    wait(0.25, sec);

    chassis.setPosition(0,0,0);
    setDriveTrainConstants();
}


// Auton SKILLS Routes
/// @brief Auton SKILLS Right [RED] - 15 Inch Robot
void AutonSkills_Right() { // Strategy: AUTON SKILLS (Right) {MIRROR Skills - Right}
    Brain.Screen.print("EXECUTING: Auton SKILLS - RIGHT");

// Initial Diagnostics
    std::cout << std::endl << std::endl << std::endl;
    std::cout << "_______________________________" << std::endl;
    std::cout << std::setw(27) << "EXECUTING: Auton Skills - Right" << std::endl;
    std::cout << "Starting Position:  " << chassis.getCurrentMotorPosition() << std::endl;
    std::cout << "Starting Heading:   " << inertial1.heading() << std::endl;


// Origin to Closest RED Loader (3 Red, 3 Blue)         {+ 5}
    chassis.driveDistance(-24, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.1, sec);
    chassis.turn(-82, 9.0);
    std::cout << inertial1.heading() << std::endl;   
    wait(0.2, sec);
    moveIntake();
    chassis.driveDistance(15, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.1, sec);
    matchLoad();
    std::cout << "Match Loading" << std::endl;
    chassis.driveDistance(2, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(1, sec);
    moveSlot();
    wait(1, sec);
    moveSlot();
    matchLoader.set(false);

// Loader to Closest RED Side Blocks (2 Blue)
    chassis.driveDistance(-16, minVoltage, 12.0, false); 
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.1, sec);
    chassis.turn(-90, 9.0);
    std::cout << inertial1.heading() << std::endl;
    wait(0.2, sec);
    chassis.driveDistance(15, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.75, sec);

// Side to Closest BLUE Loader (3 Blue, 3 Red)          {+ 5}
    chassis.driveDistance(-10, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.1, sec);
    chassis.turn(-90, 9.0);
    std::cout << inertial1.heading() << std::endl;
    wait(0.2, sec);
    chassis.driveDistance(92, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.1, sec);
    chassis.turn(-90, 9.0);
    std::cout << inertial1.heading() << std::endl;
    wait(0.2, sec);
    chassis.driveDistance(22, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    matchLoad();
    std::cout << "Match Loading" << std::endl;
    chassis.driveDistance(2, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    moveSlot();
    wait(0.1, sec);
    moveSlot();
    wait(1, sec);
    moveSlot();

// Closest BLUE Side Blocks     [2 Red]
    wait(0.1, sec);
    chassis.driveDistance(-16, minVoltage, 12.0, false); 
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.1, sec);
    chassis.turn(90, 9.0);
    std::cout << inertial1.heading() << std::endl;
    wait(0.2, sec);
    chassis.driveDistance(15, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.75, sec);


// Score Closest LONG GOAL (Right)          {+ 20}
    chassis.driveDistance(-15, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.1, sec);
    chassis.turn(90, 9.0);
    std::cout << inertial1.heading() << std::endl;
    wait(0.2, sec);
    chassis.driveDistance(20, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.1, sec);
    rise();
    outTake(); // Changed from outTake() to outTakeAll() to SCORE && AUTO-ROTATE
    outTake();
    outTake();
    outTake();
    outTake();
    fall();
    wait(0.1, sec);

// End and Check Time

/*****
// Store Center Blocks && Score Center (Lower ??)
    chassis.driveDistance(-8, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.1, sec);
    chassis.turn(-90, 9.0);
    std::cout << inertial1.heading() << std::endl;
    wait(0.2, sec);
    chassis.driveDistance(8, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.1, sec);
    chassis.turn(90, 9.0);
    std::cout << inertial1.heading() << std::endl;
    wait(0.2, sec);
    chassis.driveDistance(32, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.1, sec);
    chassis.turn(90, 9.0);
    std::cout << inertial1.heading() << std::endl;
    wait(0.2, sec);
    chassis.driveDistance(32, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    moveSlot();
    wait(0.25, sec);
    moveSlot();
    wait(0.25, sec);
    moveSlot();
    wait(0.1, sec);
    moveSlot();
    chassis.turn(-90, 9.0);
    std::cout << inertial1.heading() << std::endl;
    wait(0.2, sec);
    chassis.driveDistance(24, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.1, sec);
    chassis.turn(135, 9.0);
    std::cout << inertial1.heading() << std::endl;
    wait(0.2, sec);
    chassis.driveDistance(12, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.1, sec);
    bottomOuttakeFunction();
    moveSlot();
    bottomOuttakeFunction();
    moveSlot();
    bottomOuttakeFunction();
    moveSlot();


// Clear Red Park Zone && Park
    chassis.driveDistance(-12, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.1, sec);
    chassis.turn(-135, 9.0);
    std::cout << inertial1.heading() << std::endl;
    wait(0.2, sec);
    chassis.driveDistance(36, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.1, sec);
    chassis.turn(90, 9.0);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.2, sec);
    chassis.driveDistance(26, minVoltage, 12.0, false);
    wait(0.15, sec);
    moveSlot();
    chassis.driveDistance(-6, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    std::cout << "PARKED" << std::endl;
*****/



  // Adjust time as needed for OPTIMAL LOADING once consistent

    // Test, Tune, Adjust AS NECESSARY
    // IF Acceptable Consistency && Accuracy/Precision >> Add Center && Parking

// Store Side Blocks IF NECESSARY --- Else: Store in 15 Inch

/* Discuss Auton. Strat. && Route::
      -- Drive && Store BLUE Side Blocks                    {Store Strat. - Side Priority}
      -- Drive && Store CENTER Blocks                       {Store Strat. - Center Priority}
      -- Score in Long Goal (Left)                          {Aggro // Score Strat.}
      -- Drive && Help Secure Long Goal (Right) w/ 24 Inch  {Shield && Sword Strat.} */
}


/// @brief Auton SKILLS Left [RED] - 24 Inch Robot
void AutonSkills_Left() { // Strategy: AUTON SKILLS (Left)
    Brain.Screen.print("EXECUTING: Auton SKILLS - LEFT");

// Initial Diagnostics
    std::cout << std::endl << std::endl << std::endl;
    std::cout << "_______________________________" << std::endl;
    std::cout << std::setw(27) << "EXECUTING: Auton Skills - Left" << std::endl;
    std::cout << "Starting Position:  " << chassis.getCurrentMotorPosition() << std::endl;
    std::cout << "Starting Heading:   " << inertial1.heading() << std::endl;

// Back from Origin to Loader (Left)
    chassis.driveDistance(-36, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.1, sec);
    chassis.turnToAngle(42, minVoltage, 9.0, false);
    std::cout << inertial1.heading() << std::endl;

// Intake && Store Blocks from Loader
    moveIntake();
    chassis.driveDistance(35, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    // Match Loader Function
    wait(2, sec); // Adjust time as needed for OPTIMAL LOADING once consistent

// Reverse && Drive to Loader (Right)
    chassis.driveDistance(-35, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.1, sec);
    chassis.turn(-90, 9.0);
    std::cout << inertial1.heading() << std::endl;
    wait(0.1, sec);
    chassis.driveDistance(96, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.1, sec);
    chassis.turn(90, 9.0);
    std::cout << inertial1.heading() << std::endl;
    wait(0.1, sec);

// Intake && Store Blocks from Loader (Again)
    chassis.driveDistance(35, minVoltage, 12.0, false);
    // Match Loader Function
    wait(2, sec); // Adjust time as needed for OPTIMAL LOADING once consistent

// Choices:
    // 1) Score in Long Goal (Right) 
    // 2) Block // Score Long Goal (Left) w/ 15 Inch
    chassis.driveDistance(-32, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.1, sec);
    chassis.turn(180, 9.0);
    std::cout << inertial1.heading() << std::endl;
    chassis.driveDistance(6.5, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    rise();
    outTake(); // Changed from outTake() to outTakeAll() to SCORE && AUTO-ROTATE
    fall();

    // Test, Tune, Adjust AS NECESSARY
    // IF Acceptable Consistency && Accuracy/Precision >> Add Center && Parking

// Store Side Blocks IF NECESSARY --- Else: Store in 15 Inch

// To Be Discussed, Tuned, Finalized
    // AUTON SKILL LEFT
}



// Auton Match Routes

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
    std::cout << inertial1.heading() << std::endl;
    chassis.driveDistance(32, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.25, sec);
    chassis.turn(-45, 9.0);
    std::cout << inertial1.heading() << std::endl;
    chassis.driveDistance(36.5, minVoltage, 12.0, false);
    wait(0.25, sec);
    moveIntake();

// Load Blocks from Loader (including: Extra given Loader Blocks [6])
    // Not loading reliably, needs tuning && other systems
    chassis.driveDistance(5, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(2, sec); // Adjust time as needed for optimal loading

// Reverse to Load Side Blocks [2]
    chassis.driveDistance(-14.5, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.25, sec);
    chassis.turn(-90, 9.0);
    std::cout << inertial1.heading() << std::endl;
    wait(0.35, sec);
    // Loads BOTH Side Blocks consistently && reliably
    chassis.driveDistance(21.5, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(1.5, sec);

// Drive to RIGHT Long Goal to SCORE
    // Needs to be tested, tuned, finalized
    chassis.driveDistance(-20, minVoltage, 12.0, false);
    wait(0.25, sec);
    chassis.turn(-90, 9.0);
    std::cout << inertial1.heading() << std::endl;
    chassis.driveDistance(16.5, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    rise();
    wait(0.5, sec);
    outTake();
    std::cout << "Scoring in Left Long Goal" << std::endl;
    wait(3, sec);

// Drive to Center Goal
    fall();
    chassis.driveDistance(-5, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    chassis.turn(-90, 9.0);
    std::cout << inertial1.heading() << std::endl;
    chassis.driveDistance(22, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    chassis.turn(45, 9.0);
    std::cout << inertial1.heading() << std::endl;
    chassis.driveDistance(13, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    outTake();
    wait(5, sec);
}

/// @brief Auton Right Slot 2 [BLUE] - Write code for route within this function.
void Auton_Right2() {
    Brain.Screen.print("EXECUTING: Auton 2 - RIGHT");

  // Initial Diagnostics
    std::cout << std::endl << std::endl << std::endl;
    std::cout << "_______________________________" << std::endl;
    std::cout << std::setw(27) << "EXECUTING: Auton 1 - Right" << std::endl;
    std::cout << "Starting Position:  " << chassis.getCurrentMotorPosition() << std::endl;
    std::cout << "Starting Heading:   " << inertial1.heading() << std::endl;

    chassis.driveDistance(24, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
}

/// @brief Auton Right Slot 3 [RED]- Write code for route within this function.
void Auton_Right3() {
    Brain.Screen.print("EXECUTING: Auton 3 - RIGHT");
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
    std::cout << inertial1.heading() << std::endl;
    chassis.driveDistance(36.5, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.25, sec);
    chassis.turn(-90, 9.0);
    std::cout << inertial1.heading() << std::endl;
    wait(0.25, sec);
    moveIntake();

// Load Blocks from Loader (including: Extra given Loader Blocks [6])
    // Not loading reliably, needs tuning && other systems
    chassis.driveDistance(5, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(2, sec); // Adjust time as needed for optimal loading

// Reverse to Load Side Blocks [2]
    chassis.driveDistance(-12, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(0.25, sec);
    chassis.turn(90, 9.0);
    std::cout << inertial1.heading() << std::endl;
    wait(0.25, sec);
    // Loads BOTH Side Blocks consistently && reliably
    chassis.driveDistance(12.5, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    wait(1.5, sec);

// Drive to LEFT Long Goal to SCORE
    // Needs to be tested, tuned, finalized
    chassis.driveDistance(-15, minVoltage, 12.0, false);
    wait(0.25, sec);
    chassis.turn(90, 9.0);
    std::cout << inertial1.heading() << std::endl;
    chassis.driveDistance(22, minVoltage, 12.0, false);
    std::cout << chassis.getCurrentMotorPosition() << std::endl;
    rise();
    wait(0.5, sec);
    outTake;
    std::cout << "Scoring in Left Long Goal" << std::endl;
    wait(3, sec);
    fall();

// Drive to Park Zone & Park
    // chassis.driveDistance(-5, 3.0, 12.0, false);
    // chassis.turn(145, 9.0);
    // chassis.driveDistance(45, 3.0, 12.0, false);

    
    // For AUTON WIN POINT
}

/// @brief Auton Left Slot 2 [BLUE] - Write code for route within this function.
void Auton_Left2() {
    Brain.Screen.print("EXECUTING: Auton 2 - LEFT");
}

/// @brief Auton Left Slot 3 [RED] - Write code for route within this function.
void Auton_Left3() {
    Brain.Screen.print("EXECUTING: Auton 3 - LEFT");
}