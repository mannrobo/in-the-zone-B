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
 * @param string options The options to choose from, split by commas
 * @return int The index of the choosen option
 **/
void lcdMenu(int line, char * options) {
    int choice = 0;

    while(nLCDButtons != 2) {
        if (nLCDButtons == 1 && choice > 0) choice--;
        if (nLCDButtons == 4 && choice < 16) choice++;

        if(nLCDButtons != 0) clearLCDLine(1);

        // Display control icons
        displayLCDChar(1, 1, '<');
        displayLCDChar(1, 14, '>');


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

    displayLCDCenteredString(0, "Match Type");
    match.type = lcdMenu(1, "Standard,Driver,Prog,Rerun");

    wait1Msec(500);
    lcdClear();

    if (match.type == 0) {
        displayLCDCenteredString(0, "Pick Alliance");
        match.alliance = lcdPick(1, "Red", "Blue");
        wait1Msec(500);
        lcdClear();
    }


    displayLCDCenteredString(0, "Pick Routine");

    // Menu Tree based on Match Type
    switch(match.type) {
        case 0:
            match.routine = lcdMenu(1, "Mogo");
            break;
        case 2:
            match.routine = lcdMenu(1, "Standard");
            break;
        default: break;
    }
}
