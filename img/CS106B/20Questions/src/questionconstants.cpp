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
 */

#include "questionconstants.h"
#include <iomanip>
#include <iostream>
#include <sstream>

/*static*/ const bool QuestionConstants::USE_MEMORY_FILE = true;
/*static*/ const std::string QuestionConstants::FILE_IO_ERROR_MESSAGE = "Error: cannot access file: ";
/*static*/ const std::string QuestionConstants::INPUT_FILE_NAME_PROMPT = "Input file name?";
/*static*/ const std::string QuestionConstants::INTRO_MESSAGE =
        "Welcome to CS 106B 20 Questions!\n"
        "Think of an item, and I will guess it\n"
        "using a series of yes-or-no questions.\n"
        "I can also learn and grow smarter over time.\n";
/*static*/ const std::string QuestionConstants::LOAD_MEMORY_MESSAGE =
        "Shall I recall my previous memory state?";
/*static*/ const std::string QuestionConstants::MAIN_MENU_TEXT =
        "P)lay, R)ead, W)rite, M)erge, S)tats, Q)uit?";
/*static*/ const std::string QuestionConstants::MEMORY_FILE = "memory.txt";
/*static*/ const std::string QuestionConstants::OUTPUT_FILE_NAME_PROMPT = "Output file name?";
/*static*/ const std::string QuestionConstants::OUTRO_MESSAGE = "\nThanks for playing!";
/*static*/ const std::string QuestionConstants::READ_TREE_MESSAGE = "Reading question tree ...";
/*static*/ const std::string QuestionConstants::SAVE_MEMORY_MESSAGE = "Remembering this memory state for later ...";
/*static*/ const std::string QuestionConstants::WRITE_TREE_MESSAGE = "Writing question tree ...";

/*static*/ std::string QuestionConstants::statsString(int questions, int answers,
                                                      int minDepth, int maxDepth,
                                                      int won, int lost) {
    // stats about computer brain/tree
    std::ostringstream out;
    out << "Brain: Q" << questions << "/A" << answers;

    // stats about tree height
    out << ", Height: " << minDepth;
    if (maxDepth != minDepth) {
        out << "-" << maxDepth;
    }

    // stats about games won/lost
    int games = won + lost;
    out << ", Games: W" << won << "/L" << lost;
    if (games > 0) {
        double winPct = 100.0 * won / games;
        out << " (" << std::fixed << std::setprecision(1) << winPct << "%)";
    }
    return out.str();
}

QuestionConstants::QuestionConstants() {
    // empty
}
