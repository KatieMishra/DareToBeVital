/**
 * CS 106B/X 20 Questions
 * This client program contains a console text menu for running your game.
 *
 * You don't need to modify this instructor-provided file.
 * Your recursive functions should work with an unmodified version of this file.
 *
 * @author Marty Stepp
 * @version 2018/11/10
 * - 18au 106B version
 * @version 2016/11/13
 * - 16au 106X version
 */

#include <climits>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "simpio.h"
#include "strlib.h"
#include "questionconstants.h"
#include "questiontree.h"

// function prototype declarations
void intro();
void loadMemory(QuestionTree& tree);
void mainMenu(UI& ui, QuestionTree& tree);
void mergeTree(UI& ui, QuestionTree& tree, std::string filename);
int outro();
void readTree(QuestionTree& tree, std::string filename);
void saveMemory(QuestionTree& tree);
void stats(QuestionTree& tree);
void writeTree(QuestionTree& tree, std::ostream& output);
void writeTree(QuestionTree& tree, std::string filename);

// main function (renamed 'multimain' to allow both console and GUI mains)
int multimain(console) {
    intro();

    // create tree and UI wrapper
    ConsoleUI ui;
    QuestionTree tree;

    // load memory state if desired
    loadMemory(tree);

    // display main menu loop
    mainMenu(ui, tree);

    // save memory state if desired
    saveMemory(tree);

    return outro();
}

/*
 * Gives a printed introduction to the program.
 */
void intro() {
    std::cout << QuestionConstants::INTRO_MESSAGE << std::endl;
}

/*
 * Loads the computer's memory of questions and answers at startup.
 */
void loadMemory(QuestionTree& tree) {
    if (QuestionConstants::USE_MEMORY_FILE
            && fileExists(QuestionConstants::MEMORY_FILE)
            && getYesOrNo(QuestionConstants::LOAD_MEMORY_MESSAGE)) {
        readTree(tree, QuestionConstants::MEMORY_FILE);
    }
}

/*
 * Displays a main menu and lets the user choose options until they quit.
 */
void mainMenu(UI& ui, QuestionTree& tree) {
    while (true) {
        std::cout << std::endl;
        std::string choice = toUpperCase(trim(getLine(QuestionConstants::MAIN_MENU_TEXT)));
        if (choice == "P") {
            tree.playGame(ui);
        } else if (choice == "R") {
            std::string filename = promptUserForFile(QuestionConstants::INPUT_FILE_NAME_PROMPT);
            readTree(tree, filename);
        } else if (choice == "W") {
            std::ofstream out;
            promptUserForFile(out, QuestionConstants::OUTPUT_FILE_NAME_PROMPT);
            writeTree(tree, out);
            out.close();
        } else if (choice == "M") {
            std::string filename = promptUserForFile(QuestionConstants::INPUT_FILE_NAME_PROMPT);
            mergeTree(ui, tree, filename);
        } else if (choice == "S") {
            stats(tree);
        } else if (choice == "Q") {
            break;
        }
    }
}

/*
 * Performs a merge between the current tree and the data from the given file.
 */
void mergeTree(UI& ui, QuestionTree& tree, std::string filename) {
    std::ifstream input;
    input.open(filename.c_str());
    if (input.fail()) {
        std::cout << QuestionConstants::FILE_IO_ERROR_MESSAGE << filename << std::endl;
    } else {
        std::cout << QuestionConstants::READ_TREE_MESSAGE << std::endl;
        tree.mergeData(ui, input);
        input.close();
    }
}

/*
 * Prints end-of-program messages just before quitting out.
 */
int outro() {
    std::cout << QuestionConstants::OUTRO_MESSAGE << std::endl;
    return 0;
}

/*
 * Reads the tree from the given file on disk.
 */
void readTree(QuestionTree& tree, std::string filename) {
    std::ifstream input;
    input.open(filename.c_str());
    if (input.fail()) {
        std::cout << QuestionConstants::FILE_IO_ERROR_MESSAGE << filename << std::endl;
    } else {
        std::cout << QuestionConstants::READ_TREE_MESSAGE << std::endl;
        tree.readData(input);
        input.close();
    }
}

/*
 * Writes the tree's memory to a saved output file as the program is exiting.
 */
void saveMemory(QuestionTree& tree) {
    std::cout << std::endl;
    if (QuestionConstants::USE_MEMORY_FILE) {
        std::cout << QuestionConstants::SAVE_MEMORY_MESSAGE << std::endl;
        writeTree(tree, QuestionConstants::MEMORY_FILE);
    }
}

/*
 * Prints statistics about the tree and its nodes.
 */
void stats(QuestionTree& tree) {
    int questions = tree.countQuestions();
    int answers   = tree.countAnswers();
    int won = tree.gamesWon();
    int lost = tree.gamesLost();
    int minDepth = 0;
    int maxDepth = 0;
    tree.answerRange(minDepth, maxDepth);
    std::cout << QuestionConstants::statsString(
                     questions, answers,
                     minDepth, maxDepth,
                     won, lost) << std::endl;
}

/*
 * Writes the tree's memory to the given output stream.
 */
void writeTree(QuestionTree& tree, std::ostream& output) {
    std::cout << QuestionConstants::WRITE_TREE_MESSAGE << std::endl;
    tree.writeData(output);
}

/*
 * Writes the tree's memory to the given output file.
 */
void writeTree(QuestionTree& tree, std::string filename) {
    std::ofstream output;
    output.open(filename.c_str());
    if (output.fail()) {
        std::cout << QuestionConstants::FILE_IO_ERROR_MESSAGE << filename << std::endl;
    } else {
        writeTree(tree, output);
        output.close();
    }
}
