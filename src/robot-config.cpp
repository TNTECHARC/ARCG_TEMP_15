#include "robot-config.h"

////////////// DO NOT REMOVE //////////////

// Defines the Brain and Controller
brain Brain;
controller Controller1;

///////////////////////////////////////////

//////////// Drive Train Motors ////////////
// From Back View
motor R1 = motor(PORT12, ratio6_1, true);
motor R2 = motor(PORT13, ratio6_1, true);
motor R3 = motor(PORT14, ratio6_1, true);
motor R4 = motor(PORT15, ratio6_1, true);
motor L1 = motor(PORT17, ratio6_1, false);
motor L2 = motor(PORT18, ratio6_1, false);
motor L3 = motor(PORT19, ratio6_1, false);
motor L4 = motor(PORT20, ratio6_1, false);

///////////////////////////////////////////

//System
motor revolver = motor(PORT10, ratio36_1, false);

motor outtakeLeft = motor(PORT1, ratio18_1, true);
motor outtakeRight = motor(PORT11, ratio18_1, false);
motor_group outtake = motor_group(outtakeLeft, outtakeRight);

motor intakeLeft = motor(PORT2, ratio36_1, true);
motor intakeRight = motor(PORT9, ratio36_1, false);
motor_group intake = motor_group(intakeLeft, intakeRight);

//////////// Odometry Sensors ////////////
//TODO: CHANGE THE NAMES OF THESE
rotation rotation1 = rotation(PORT9);
rotation rotation2 = rotation(PORT2); 

/////////////////////////////////////////

//////////// Inertial Sensors ////////////

inertial inertial1 = inertial(PORT8);

/////////////////////////////////////////

led liftR = led(Brain.ThreeWirePort.A);
led liftL = led(Brain.ThreeWirePort.B);
led matchLoader = led(Brain.ThreeWirePort.C);
led extendo = led(Brain.ThreeWirePort.G);

/////////////////////////////////////

optical backColorSensor = optical(PORT7);
optical middleColorSensor = optical(PORT6);
optical frontColorSensor = optical(PORT16);

motor bottomOuttake = motor(PORT3, ratio36_1, false);