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