
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
void driveDistance(int inches) {
    robot.driveDirect = true;
    driveReset();

    // Calculate the movement
    int targetTicks = inches;
    int deceleratePeriod = clamp(targetTicks / 2, 0, 800);
    targetTicks -= deceleratePeriod;
    int startTicks  = SensorValue[leftDrive];
    writeDebugStreamLine("driveDistance: %d %d", targetTicks, startTicks);

    while(targetTicks - SensorValue[leftDrive] > 50) {
        drive(127, 127);
    }

    // Stop the drive after we've completed the distance
    drive(0, 0);
    robot.driveDirect = false;
}

/**
 * Turns a set number of degrees
 */

int rotationTicks() {
    return sgn(SensorValue[leftDrive]) * (abs(SensorValue[leftDrive]) + abs(SensorValue[rightDrive])) / 2;
}

/**
 * Turns a specified number of rotations
 * float rotations The number of rotations to turn
 **/
void turn(float rotations) {
    driveReset();
    int targetTicks = 440 * rotations;
    writeDebugStreamLine("turn: %d %d", targetTicks, rotationTicks());
    while (abs(rotationTicks()) < abs(targetTicks)) {
        float factors = sgn(rotations) * profileTrapezoid(0, targetTicks, rotationTicks());
        drive(30 * factors + 30, -30 * factors - 30);
    }
    drive(0, 0);
}



/* Section 2: Mobile Goal Lift */
void mogoSet(int value) {
    motorTarget[port2] = value;
    motorTarget[port3] = value;
}

void mogoHandle() {
    if(robot.mogo == UP && SensorValue[mogoLeft] > 600) {
        mogoSet(127);
        robot.mogoMoving = true;
    } else if (robot.mogo == DOWN && SensorValue[mogoLeft] < 2450) {
        mogoSet(-127);
        robot.mogoMoving = true;
    } else {
        mogoSet(0);
        robot.mogoMoving = false;
    }
}

void untilMogoDone() {
    while(robot.mogoMoving) {};
    return;
}


void mogoUp() {
    robot.mogo = UP;
    robot.mogoMoving = true;
    untilMogoDone();
}
void mogoDown() {
    robot.mogo = DOWN;
    robot.mogoMoving = true;
    untilMogoDone();
}

task handleAll() {
    while(true) {
        mogoHandle();
        driveHandle();
        motorHandle();
        wait1Msec(20);
    }
}
