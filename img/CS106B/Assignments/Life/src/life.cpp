/* Written by Katie Mishra for CS106B, Fall 2018
 * This file plays Conway's Game of Life. From the user, it takes in a file representing a
 * colony of cells. Then the user can tick/animate the colony to represent generations of cells.
 * Cells die/appear based on their number of neighbors. Gameplay is represented both through a
 * console and a GUI. Game continues until the user quits.
 */

#include <iostream>
#include <fstream>
#include "strlib.h"
#include "console.h"
#include "filelib.h"
#include "map.h"
#include "grid.h"
#include "simpio.h"
#include "lifegui.h"
using namespace std;

void printWelcomeMessage();
void openFile(Grid<string>&);
void printGrid(const Grid<string>&);
string getUserAdvanceChoice();
int getNumFrames();
void tickForward(Grid<string>&, Grid<string>&);
void createNeighborMap(int row, int col, const Grid<string>& colony, Map<string, int>& directionMap);
int evaluateNeighbors(int row, int col, const Grid<string>& colony, const Map<string, int>& directionMap);
bool preferenceLoadAnotherFile();

int main() {
    LifeGUI::initialize();
    printWelcomeMessage();
    bool loadAnotherFile = true;
    while (loadAnotherFile) {
        Grid<string> colony;
        openFile(colony);
        printGrid(colony);
        string advance = getUserAdvanceChoice();
        while (advance != "q") {
            if (advance == "t") {
                Grid<string> newColony(colony.height(), colony.width());
                tickForward(colony, newColony);
                colony = newColony;
            } else if (advance == "a") {
                for (int numFrames = getNumFrames(); numFrames > 0; numFrames--) {
                    clearConsole();
                    pause(100);
                    Grid<string> newColony(colony.height(), colony.width());
                    tickForward(colony, newColony);
                    colony = newColony;
                }
            }
            printGrid(colony);
            advance = getUserAdvanceChoice();
        }
        loadAnotherFile = preferenceLoadAnotherFile();
    }
    cout << "Have a nice life!" << endl;
    LifeGUI::shutdown();
    return 0;
}

//Prints intro message before game starts.
void printWelcomeMessage() {
    cout << "Welcome to the CS 106B/X Game of Life!" << endl
         << "This program simulates the lifecycle of a bacterial colony." << endl
         << "Cells (X) live and die by the following rules:" << endl
         << "* A cell with 1 or fewer neighbors dies." << endl
         << "* Locations with 2 neighbors remain stable." << endl
         << "Cells (X) live and die by the following rules:" << endl
         << "Welcome to the CS 106B/X Game of Life!" << endl;
}

 /* Takes an unitialized Grid, to store the colony in, as a parameter.
  * Opens file on user input, tests if file exists, and if so, populates Grid with
  * strings from file.*/
void openFile(Grid<string>& colony) {
    ifstream infile;
    while (colony.isEmpty()) {
        string file = promptUserForFile(infile, "Grid input file name? ");
        if (fileExists(file)) {
            string line;
            getline(infile, line);
            int numRows = stringToInteger(line);
            getline(infile, line);
            int numCols = stringToInteger(line);
            colony.resize(numRows, numCols);
            LifeGUI::resize(numRows, numCols);

            for (int row = 0; row < numRows; row++) {
                getline(infile, line);
                for (int col = 0; col < numCols; col++) {
                    colony[row][col] = line[col];
                    if (colony[row][col] == "X") {
                        LifeGUI::fillCell(row,col);
                    }
                }
            }
        } else {
            cout << "Unable to open that file.  Try again." << endl;
        }
    }
    LifeGUI::repaint();
    infile.close();
}

/*  Takes a populated Grid of cells as a parameter.
 *  Loops over Grid to print colony to console. */
void printGrid(const Grid<string>& COLONY) {
    for (int row = 0; row < COLONY.height(); row++) {
        for (int col = 0; col < COLONY.width(); col++) {
            cout << COLONY[row][col];
        }
        cout << endl;
     }
}

/*  Takes user input as to how to advance colony or quit game.
 *  Returns 't' to tick forward a generation,
 *            'a' to animate generations, or
 *            'q' to quit */
string getUserAdvanceChoice() {
    bool properlyFormattedChoice = false;
    while (!properlyFormattedChoice) {
        string input = getLine("a)nimate, t)ick, q)uit? ");
        if (input.empty() || toLowerCase(input) == "t") {
           return "t";
        } else if (toLowerCase(input) == "a") {
           return "a";
        } else if (toLowerCase(input) == "q") {
           return "q";
        }
        cout << "Invalid format. Please try again." << endl;
    }
    return "";
}

/*  Takes user input as to the number of frames to animate.
 *  Returns number of frames */
int getNumFrames() {
    int numFrames = getInteger("How many frames? ", "Illegal integer format. Try again.");
    return numFrames;
}

/*  Takes in parameters of a Grid representing the current state of colony
 *  and a Grid to store the new state of the colony.
 *  Ticks colony forward a generation after evaluating the cell's number
 *  of neighbors. */
void tickForward(Grid<string>& colony, Grid<string>& newColony) {
    LifeGUI::clear();
    for (int row = 0; row < colony.height(); row++) {
        for (int col = 0; col < colony.width(); col++) {
            Map<string, int> directionMap;
            createNeighborMap(row, col, colony, directionMap);
            int numNeighbors = evaluateNeighbors(row, col, colony, directionMap);
            if (numNeighbors == 0 || numNeighbors == 1 || numNeighbors >= 4) {
                newColony[row][col] = "-";
            } else if (numNeighbors == 3) {
                newColony[row][col] = "X";
                LifeGUI::fillCell(row, col);
            } else if (numNeighbors == 2) {
                if (colony[row][col] == "X") {
                    LifeGUI::fillCell(row,col);
                }
                newColony[row][col] = colony[row][col];
            }
        }
    }
    LifeGUI::repaint();
}


/*  Takes parameters as to the row that cell is in, col that cell is in,
 *  Grid representing current state of colony, and an empty map of neighboring rows/cols to cell.
 *  Fills the map showing which rows/cols the cell's neighbors are in.*/
void createNeighborMap(int row, int col, const Grid<string>& COLONY, Map<string, int>& directionMap) {

    //inital values of map account for wrapping around colony on all sides
    directionMap["topRowNum"] = COLONY.height() - 1;
    directionMap["rightColNum"] = 0;
    directionMap["bottomRowNum"] = 0;
    directionMap["leftColNum"] = COLONY.width() - 1;

    if (COLONY.inBounds(row-1,col)) {
        directionMap["topRowNum"] = row - 1;
    }
    if (COLONY.inBounds(row,col+1)) {
        directionMap["rightColNum"] = col + 1;
    }
    if (COLONY.inBounds(row+1,col)) {
        directionMap["bottomRowNum"] = row + 1;
    }
    if (COLONY.inBounds(row,col-1)) {
        directionMap["leftColNum"] = col - 1;
    }
}

/*  Takes parameters as to the row that cell is in, col that cell is in,
 *  Grid representing current state of colony, and a populated map of neighboring rows/cols to cell.
 *  Returns number of neighbors that the cell has. */
int evaluateNeighbors(int row, int col, const Grid<string>& COLONY, const Map<string, int>& DIRECTION_MAP) {
    int numLiveNeighbors = 0;
    //From colony, gets the string at each neighboring location to store in a vector
    Vector<string> neighborVector {COLONY[DIRECTION_MAP["topRowNum"]][DIRECTION_MAP["leftColNum"]],
            COLONY[DIRECTION_MAP["topRowNum"]][col],
            COLONY[DIRECTION_MAP["topRowNum"]][DIRECTION_MAP["rightColNum"]],
            COLONY[row][DIRECTION_MAP["rightColNum"]],
            COLONY[DIRECTION_MAP["bottomRowNum"]][DIRECTION_MAP["rightColNum"]],
            COLONY[DIRECTION_MAP["bottomRowNum"]][col],
            COLONY[DIRECTION_MAP["bottomRowNum"]][DIRECTION_MAP["leftColNum"]],
            COLONY[row][DIRECTION_MAP["leftColNum"]]};

    for (string neighbor : neighborVector) {
        if (neighbor == "X") {
            numLiveNeighbors++;
        }
    }
    return numLiveNeighbors;
}

/*  User input as to whether they want to load another colony.
 *  Returns True if 'y', False if 'n'. */
bool preferenceLoadAnotherFile() {
    return getYesOrNo("Load another file? (y/n) ", "Please type a word that starts with 'Y' or 'N'.");
}
