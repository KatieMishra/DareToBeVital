/*
 * CS 106B/X 20 Questions
 * This file implements the graphical user interface for 20 Questions.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @version 2018/11/10
 * - initial version for 106B 18au
 *
 * @author Marty Stepp (18au)
 *
 * Known open issue: Leaks memory for all GObjects created.  Oops.
 */

#include "questiongui.h"

// main function to run the actual program
int multimain(GUI) {
    // create
    QuestionGUI* gui = QuestionGUI::instance();
    gui->initialize();
    return 0;
}

// this is only here so it will compile
int main() {
    return 0;
}
