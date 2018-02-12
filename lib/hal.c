
/**
 * hal.c - Hardware Abstraction layer, abstacts motors & sensors into a set of accessor functions
 **/

// Slight crinkle in HAL philosophy, in case where more advanced motor algorithmns are being applied
#include "motor.c"
#include "util.c"
#include "pid.c"
#include "profile.c"

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
    motorTarget[port4] = -robot.leftDrive;
    motorTarget[port6] = -robot.leftDrive;
    motorTarget[port8] = -robot.leftDrive;

    motorTarget[port5] = robot.rightDrive;
    motorTarget[port7] = robot.rightDrive;
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
        int syncspeed = pidCalculate(drivePID, driveOffset()),
            basespeed = 90 * profileTrapezoid(targetTicks, SensorValue[leftDrive], 1/8); 
        drive(basespeed - syncspeed, basespeed + syncspeed);
    }

    // Stop the drive after we've completed the distance
    drive(0, 0);
}

/* Section 2: Mobile Goal Lift */
void mogoSet(int value) {
    motorTarget[port2] = value;
    motorTarget[port3] = value;
}

void mogoHandle() {
    if(robot.mogo == UP && SensorValue[mogoLeft] > 150) {
        mogoSet(127);
    } else if (robot.mogo == DOWN && SensorValue[mogoLeft] < 2400) {
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
