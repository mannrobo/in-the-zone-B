/**
 * lcd.c - LCD Selection Library
 **/


void lcdClear() {
    clearLCDLine(0);
    clearLCDLine(1);
}



/**
 * UI Component: Pick between two options, using left and right buttons to choose, and center to confirm
 * @param char * leftOption The option to display on the left
 * @param char * rightOption The option to display on the right
 * @return int The index of the choosen option, 0 for the left option, 1 for the right
 **/
int lcdPick(int line, char * leftOption, char * rightOption) {
    int choice = 0;

    string indicator = "*";
    string spacer = "  ";

    while(nLCDButtons != 2) {
        string buffer = "";
        if (nLCDButtons == 1) choice = 0;
        if (nLCDButtons == 4) choice = 1;

        if(!choice) strncat(buffer, indicator, 1);
        strncat(buffer, leftOption, sizeof(leftOption));
        strncat(buffer, spacer, 2);
        if(choice) strncat(buffer, indicator, 1);
        strncat(buffer, rightOption, sizeof(rightOption));

        displayLCDCenteredString(line, buffer); // All UI components, by default, display on line 1
    }
    lcdClear();
    return choice;

}


// Basic Definition of a String, used to pass to functions
typedef struct {
   string value;
} String;

/**
 * UI Component: Pick between multiple options
 * @param int line The line to display on
 * @param String * options The array of options. Construct with String options = {"Option 1", "Option 2"}
 * @param int arrayLength The total number of options
 * @return int The index of the choosen option
 **/
void lcdMenu(int line, String * options, int arrayLength) {
    int choice = 0;

    while(nLCDButtons != 2) {
        if (nLCDButtons == 1 && choice > 0) choice--;
        if (nLCDButtons == 4 && choice < arrayLength) choice++; // sizeof() returns byte size, 20 bytes is a string

        if(nLCDButtons != 0) clearLCDLine(1);

        // Display control icons
        displayLCDChar(1, 1, '<');
        displayLCDChar(1, 14, '>');

        displayLCDString(1, 3, options[choice].value);

		wait1Msec(100);

    }

}

/**
 * Holds the code for autonomous selection, etc.
 * Run in pre_auton();
 **/
void robotConfigure() {
    lcdClear();

    displayLCDCenteredString(0, "Lightning McQueen");
    displayLCDCenteredString(1, "3796B");

    wait1Msec(500);
    lcdClear();

    String matchTypes[4] = { "Standard", "Driver", "Prog", "Rerun" };
    displayLCDCenteredString(0, "Match Type");
    match.type = lcdMenu(1, matchTypes, 4);

    wait1Msec(500);
    lcdClear();

    if (match.type == 0) {
        displayLCDCenteredString(0, "Pick Alliance");
        match.alliance = lcdPick(1, "Red", "Blue");
        wait1Msec(500);
        lcdClear();
    }

    // Menu Tree based on Match Type
    switch(match.type) {
        displayLCDCenteredString(0, "Pick Routine");
        case 0:
            String routines[2] = { "Mogo", "Block" };
            match.routine = lcdMenu(1, routines, 2);
            break;
        case 2:
            String routines[1] = { "Standard" };
            match.routine = lcdMenu(1, routines, 1);
            break;
        default: break;
    }
}
