
/**
 * hal.c - Hardware Abstraction layer, abstacts motors & sensors into a set of accessor functions
 **/

// Slight crinkle in HAL philosophy, in case where more advanced motor algorithmns are being applied
#include "motor.c"
#include "util.c"
#include "pid.c"

/* Section 1: Drive */
void drive(int left, int right) {
    robot.leftDrive = left;
    robot.rightDrive = right;
}

void driveReset() {
    SensorValue[leftDrive] = 0;
    SensorValue[rightDrive] = 0;
}

int driveOffset() {
    return SensorValue[leftDrive] - SensorValue[rightDrive];
}

void driveHandle() {
    motorTarget[port2] = robot.leftDrive;
    motorTarget[port3] = robot.leftDrive;

    motorTarget[port8] = robot.rightDrive;
    motorTarget[port9] = robot.rightDrive;
}

/**
 * Drives a set number of specified inches
 */
pidConfiguration drivePID;
void driveDistance(int inches) {
    pidConfigure(drivePID, 0, 0, 0);
    pidReset(drivePID);

    int targetTicks = inchesToTicks(inches, 3.25, 3, TORQUE);

    while(SensorValue[leftDrive] != targetTicks || SensorValue[rightDrive] != targetTicks) {
        int syncspeed = pidCalculate(drivePID, driveOffset());
        drive(80 - syncspeed, 80 + syncspeed);
    }

    // Stop the drive after we've completed the distance
    drive(0, 0);
}

/* Section 2: Mobile Goal Lift */
void mogoSet(int value) {
    motorTarget[port5] = value;
    motorTarget[port6] = value;
}

void mogoHandle() {
    if(robot.mogo == UP && SensorValue[mogoLift] > 1500) {
        mogoSet(127);
    } else if (robot.mogo == DOWN && SensorValue[mogoLift] < 3000) {
        mogoSet(-127);
    } else {
        mogoSet(0);
    }
}

void mogoUp() {
    robot.mogo = UP;
}
void mogoDown() {
    robot.mogo = DOWN;
}


task handleAll() {
    while(true) {
        mogoHandle();
        driveHandle();
        motorHandle();
        wait1Msec(20);
    }
}
