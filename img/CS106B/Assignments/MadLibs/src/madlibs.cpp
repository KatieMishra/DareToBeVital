/* Katie Mishra, CS106B Fall 2018
 * This program lets users play MadLibs. After being prompted
 * to enter a file, they will be asked to enter various types of
 * words in order to complete the story. At the end, the story is displayed
 * back to them. They can then choose to play another game or end, and
 * see stats on how many total and average placeholders they replaced. */

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "simpio.h"
#include "strlib.h"
#include "iomanip"
using namespace std;

void printWelcomeMessage();
void openFile(string& file);
bool preferenceLoadAnotherFile();
int askForAdjectives(string& story, string& file);
void displayStory(string& story, int numPlaceholders);
void printEndMessage(int numStoriesCreated, int totalPlaceholders);

//Lays out the structure of the game and calls corresponding functions to complete MadLibs
int main() {
    printWelcomeMessage();
    ifstream infile;
    bool loadAnotherFile = true;
    int numStoriesCreated = 0;
    int totalPlaceholders = 0;
    string story = "";
    string file;

    //runs while the user wants to play another game
    while (loadAnotherFile) {
        openFile(file);
        int numStoryPlaceholders = askForAdjectives(story, file);
        displayStory(story, numStoryPlaceholders);
        loadAnotherFile = preferenceLoadAnotherFile();
        totalPlaceholders += numStoryPlaceholders;
        numStoriesCreated++;
    }
    printEndMessage(numStoriesCreated, totalPlaceholders);
    return 0;
}

//Prints intro message before game starts.
void printWelcomeMessage() {
    cout << "Welcome to the CS 106B MadLibs!" << endl
         << "I will ask you to provide various words." << endl
         << "and phrases to fill in a story." << endl
         << "At the end, I will display your story to you." << endl;
}

/* Parameter -- ifstream object to open file
 * Repeatedly asks the user for an input file until a valid
file is given. */
void openFile(string& file) {
   ifstream infile;
   cout << endl;
   file = promptUserForFile(infile, "Mad Lib input file? ");
   cout << endl;
}

/*Parameter -- a reference to a string to store the story in
 *Function loops over each line in the file. Finds placeholders
 * and asks user to replace them with new words.
 * Return -- number of placeholders in the story */
int askForAdjectives(string& completedStory, string& file) {
    ifstream infile;
    string line;
    int numPlaceholders = 0;
    infile.open(file);

    //reads the file line by line
    while (getline(infile,line)) {

        //determines whether there's a placeholder in the line, if both '<' and '>' are in the line
        while (line.find("<") != std::string::npos && line.find(">") != std::string::npos) {
            int firstBracket = line.find("<")+1;
            int secondBracket = line.find(">");

            //Only replaces words if the '<' bracket is before the '>' bracket
            if (firstBracket < secondBracket) {
                int length = secondBracket - firstBracket;
                string typeOfWord = line.substr(firstBracket,length);
                string firstChar = toLowerCase(typeOfWord.substr(0,1));
                string promptLeadIn = "a";
                if (firstChar == "a" || firstChar == "e" || firstChar == "i"
                    || firstChar == "o" || firstChar == "u") {
                    promptLeadIn = "an";
                }
                string prompt = "Please type " + promptLeadIn + " " + typeOfWord + ": ";
                string word = getLine(prompt);
                line.replace(line.find("<"),length+2,word);
                numPlaceholders++;
           }
        }
        completedStory += line;
    }
    infile.close();
    return numPlaceholders;
}

/*Parameters -- filled in mad libs story, number of placeholders in story
 * Prints story and stats to console.*/
void displayStory(string& story, int numPlaceholders) {
    cout << endl
         << "Your Mad Lib story:" << endl
         << story << endl
         << endl
         << numPlaceholders << " placeholder(s) replaced." << endl;
}

/*  User input as to whether they want to play another game.
 *  Return -- True if 'y', False if 'n'. */
bool preferenceLoadAnotherFile() {
    return getYesOrNo("Load another file? (y/n) ", "Please type a word that starts with 'Y' or 'N'.");
}

/* Parameters -- number of stories user has created, total words they've filled in for all
 * the stories.
 * Prints an ending message showing the user how many stories they created, including how many
 * words they filled in and the average words per story. */
void printEndMessage(int numStoriesCreated, int totalPlaceholders) {
    double averagePlaceholders = totalPlaceholders/numStoriesCreated;
    cout << endl
         << "Mad Lib stories you created: " << numStoriesCreated << endl
         << "Total placeholders replaced: " << totalPlaceholders << " (" << fixed
         << setprecision(1) << averagePlaceholders << " per story)" << endl;
}
