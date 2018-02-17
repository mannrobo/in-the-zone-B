/**
 * skills.c - Holds programming skills routines (60 second)
 */


void skillsFoward() {
    drive(127, 127);
}

string skillsRoutines[] = { "Forward" };
void goSkills(int routine) {
    switch(routine) {
        default: case 0:
            skillsFoward();
            break;
    }
}
