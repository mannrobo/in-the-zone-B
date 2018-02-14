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
} robotState;

robotState robot;


/**
 * Update robot state based on controller values
 */
void updateState() {
    if (vexRT[Btn6U] || nLCDButtons == kButtonLeft || vexRT[Btn5U]) robot.mogo = UP;
    if (vexRT[Btn6D] || nLCDButtons == kButtonRight || vexRT[Btn5D]) robot.mogo = DOWN;

    int forward = abs(vexRT[Ch3]) > 25 ? vexRT[Ch3] : 0,
        turn = abs(vexRT[Ch4]) > 30 ? vexRT[Ch4] * 0.65 : 0,
        left = forward + turn,
        right = forward - turn;

    robot.leftDrive  = sgn(left)  * rescaleTo(127, abs(left), abs(right), 0);
    robot.rightDrive = sgn(right) * rescaleTo(127, abs(left), abs(right), 1);
}

/**
 * State Codes
 * Short, textual representations of the robot's state. They take the form:
 * <mogoLift>:<leftDrive>:<rightDrive>
 * Examples:
 *   0:127:-127
 *   0:0:0
 */

void outputStateCode() {
    string out;
    sprintf(out, "%d:%d:%d", robot.mogo == UP ? 0 : 1, robot.leftDrive, robot.rightDrive);
    displayLCDString(1, 0, out);
}

void activateStateCode(char * code) {
    char state[STRTOK_MAX_TOKEN_SIZE];
    char * ptr = &state;
    char * sep = ":";
    while (true) {
        if (!strtok(code, ptr, sep)) break;
    }
    robot.mogo = state[0] == 0 ? UP : DOWN;
    robot.leftDrive = (int) state[1];
    robot.rightDrive = (int) state[2];

}
