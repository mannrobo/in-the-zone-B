/**
 * auton.c - Holds autonomous routines (15 second)
 */

void autonMogo() {
    mogoDown();
    untilMogoDone();
    driveDistance(1500);
}

void autonBlock() {

}

void autonSpin() {
    robot.debugDisplay = 1;
    driveDistance(36);
    wait1Msec(1000);
    turn(0.5);
    wait1Msec(1000);
    driveDistance(36);
    wait1Msec(1000);
    turn(0.5);
}

string autonRoutines[] = { "Mogo", "Block", "SpinTest" };
void goAuton(int routine) {
    switch(routine) {
        default: case 0:
            autonMogo();
            break;
        case 1:
            autonBlock();
            break;
        case 2:
            autonSpin();
            break;
    }
}
