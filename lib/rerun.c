/**
 * rerun.c - Super basic rerun system, warning: NOT GENERALIZED ;-)
 */

#include "util.c"
#pragma systemFile

typedef struct {
    int type; // 0 = standard, 1 = driver, 2 = programming, 3 = record rerun
    int routine; // See /routines
	int alliance; // 0 = red, 1 = blue
} matchState;

matchState match;

enum mogoState {
    UP, DOWN
};

typedef struct robotState {
    mogoState mogo;
    int leftDrive;
    int rightDrive;
    bool driveDirect; // In autonomous, we would likely want to forgo slew rate, motion profiling will take care of its role

    bool mogoMoving;
    bool driveMoving;

    int debugDisplay; // Refer to debugValues() task for more information
} robotState;

robotState robot;



mogoState lastMogoState;
string rerunAction;

void rerunReset() {
    ClearTimer(T1);
    lastMogoState = robot.mogoState;
    driveReset();
    rerunAction = "NONE";
}

/**
 * Because purely time based rerun is quite unreliable, 
 * this rerun system is based off of setting instructions
 * via the robot. It's basically a faster way of writing
 * autonomous functions. Ideally, the output of the rerun 
 * would be code we can copy paste into routines/ whole-cloth
 * 
 * There are four different actions the robot can take in autonomous:
 *  - Drive (straight or slightly arced)
 *  - Turn
 *  - Mobile Goal Up/Down
 *  - Wait
 * 
 * Because the set is so simple (and rarely would we want to do any of those for a specific length of time) the
 * easiest/most accurate rerun system would be one that has the driver perform the action, and then 
 * press a button to indicate the action is complete, performing one action at a time through the routine.
 **/
void rerunHandle() {
    robot.debugDisplay = -1;
    if (vexRT[Btn7D]) {
        // Display the changed action in the frame
        if (lastMogoState != robot.mogoState) {
            rerunAction = "MOGO";
            writeDebugStreamLine("mogo%s();", robot.mogoState == UP ? "UP" : "DOWN");
        } else if (abs(SensorValue[leftDrive]) > 200 && SensorValue[leftDrive] + SensorValue[rightDrive < 100]) { // Turning
            rerunAction = "TURN";
            writeDebugStreamLine("// Turn")
        } else if (abs(SensorValue[leftDrive]) > 200 && abs(SensorValue[rightDrive]) > 200) {
            rerunAction = "DRIVE";
            writeDebugStreamLine("driveDistance(%d, %d)", SensorValue[leftDrive], SensorValue[leftDrive]);
        } else {
            rerunAction = "WAIT";
            writeDebugStreamLine("wait1MSec(%d)", time1[T1])
        }
        rerunReset();
    } else if (vexRT[Btn7L]) { // Cancel the previous action
        rerunReset();
    }
}