#include "robot-config.h"

////////////// DO NOT REMOVE //////////////

// Defines the Brain and Controller
brain Brain;
controller Controller1;

///////////////////////////////////////////

//////////// Drive Train Motors ////////////

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
motor outtake1 = motor(PORT1, ratio18_1, true);
motor outtake2 = motor(PORT11, ratio18_1, false);
motor_group outtake = motor_group(outtake1, outtake2);

motor intake1 = motor(PORT2, ratio36_1, false);
motor intake2 = motor(PORT5, ratio36_1, true); 
motor_group intake = motor_group(intake1, intake2);

//////////// Odometry Sensors ////////////
//TODO: CHANGE THE NAMES OF THESE
rotation rotation1 = rotation(PORT9);
rotation rotation2 = rotation(PORT2); //FIXME

/////////////////////////////////////////

//////////// Inertial Sensors ////////////

inertial inertial1 = inertial(PORT9);

/////////////////////////////////////////

led liftR = led(Brain.ThreeWirePort.A);
led liftL = led(Brain.ThreeWirePort.B);
led matchLoader = led(Brain.ThreeWirePort.H);

/////////////////////////////////////

optical backColorSensor = optical(PORT6);
optical middleColorSensor = optical(PORT7);
optical frontColorSensor = optical(PORT16);

motor bottomOuttake = motor(PORT3, ratio18_1, false);