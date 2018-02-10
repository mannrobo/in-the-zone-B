/**
 * auton.c - Holds autonomous routines (15 second)
 */

void autonMogo() {

}

void autonBlock() {
    
}


string autonRoutines[] = { "Mogo", "Block" };
void goAuton(int routine) {
    switch(routine) {
        default: case 0:
            autonMogo();
            break;
        case 1:
            autonBlock();
            break;
    }
}