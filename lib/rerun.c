/**
 * rerun.c - Super basic rerun system, warning: NOT GENERALIZED ;-)
 */

#include "util.c"
#pragma systemFile



mogoState lastMogoState;
string rerunAction;

void rerunReset() {
    clearTimer(T1);
    lastMogoState = robot.mogo;
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
        if (lastMogoState != robot.mogo) {
            rerunAction = "MOGO";
            writeDebugStreamLine("mogo%s();", robot.mogo == UP ? "UP" : "DOWN");
        } else if (abs(SensorValue[driveLeft]) > 200 && SensorValue[driveLeft] + SensorValue[driveRight] < 100) { // Turning
            rerunAction = "TURN";
            writeDebugStreamLine("// Turn");
        } else if (abs(SensorValue[driveLeft]) > 200 && abs(SensorValue[driveRight]) > 200) {
            rerunAction = "DRIVE";
            writeDebugStreamLine("driveDistance(%d, %d)", SensorValue[driveLeft], SensorValue[driveLeft]);
        } else {
            rerunAction = "WAIT";
            writeDebugStreamLine("wait1MSec(%d)", time1[T1]);
        }
        rerunReset();
    } else if (vexRT[Btn7L]) { // Cancel the previous action
        rerunReset();
    }
}
