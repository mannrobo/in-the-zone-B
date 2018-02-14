
/**
 * hal.c - Hardware Abstraction layer, abstacts motors & sensors into a set of accessor functions
 **/

// Slight crinkle in HAL philosophy, in case where more advanced motor algorithmns are being applied
#include "motor.c"
#include "pid.c"
#include "profile.c"

#pragma systemFile

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
    robot.driveDirect = true;

    // Test: Just letting it drift for 800 ticks is pretty accurate
    int targetTicks = inchesToTicks(inches, 3.25, 3, TORQUE) - 800;
    int startTicks  = SensorValue[leftDrive];
    writeDebugStreamLine("driveDistance: %d %d", targetTicks, startTicks);

    while(targetTicks - SensorValue[leftDrive] > 50) {
        float mult = profileTrapezoid(startTicks, targetTicks, SensorValue[leftDrive], 0.3);
        int syncspeed = pidCalculate(drivePID, driveOffset()),
            basespeed = 127
        writeDebugStreamLine("%d:%d", basespeed, syncspeed);

        drive(basespeed - syncspeed, basespeed + syncspeed);
        wait1Msec(20);
    }

    // Stop the drive after we've completed the distance
    drive(0, 0);
    robot.driveDirect = false;
}

/* Section 2: Mobile Goal Lift */
void mogoSet(int value) {
    motorTarget[port2] = value;
    motorTarget[port3] = value;
}

void mogoHandle() {
    if(robot.mogo == UP && SensorValue[mogoLeft] > 400) {
        mogoSet(127);
        robot.mogoMoving = true;
    } else if (robot.mogo == DOWN && SensorValue[mogoLeft] < 3000) {
        mogoSet(-127);
        robot.mogoMoving = true;
    } else {
        mogoSet(0);
        robot.mogoMoving = false;
    }
}

void mogoUp() {
    robot.mogo = UP;
}
void mogoDown() {
    robot.mogo = DOWN;
}

void untilMogoDone() {
    while(robot.mogoMoving) {};
    return;
}

task handleAll() {
    while(true) {
        mogoHandle();
        driveHandle();
        motorHandle();
        wait1Msec(20);
    }
}
