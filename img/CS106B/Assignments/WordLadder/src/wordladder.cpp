/* Katie Mishra, Fall 2018, CS106B.
 * From two words given by the user, Word Ladder prints the shortest
 * ladder from the first word to the second word. Ladders are created
 * by changing one character in the word at a time to create a new, valid
 * word until reaching the target word.
 */

#include <iostream>
#include "console.h"
#include "lexicon.h"
#include "filelib.h"
#include "simpio.h"
#include "queue.h"
#include "stack.h"
#include "string.h"
using namespace std;

void printWelcomeMessage();
void openFile(Lexicon& dict);
bool getWords(string&, string&, const Lexicon&);
void getLadder(string, string, const Lexicon&, Stack<string>&);
void printLadder(string, string, Stack<string>&);

int main() {
    printWelcomeMessage();
    Lexicon dict;
    openFile(dict);
    string word1;
    string word2;
    while (getWords(word1, word2, dict)) {
        Stack<string> wordLadder;
        getLadder(word1, word2, dict, wordLadder);
        printLadder(word1, word2, wordLadder);
    }
    cout << "Have a nice day." << endl;
    return 0;
}

//Prints welcome message to Word Ladder.
void printWelcomeMessage() {
    cout << "Welcome to CS 106B/X Word Ladder!" << endl
         << "Please give me two English words, and I will convert the" << endl
         << "first into the second by modifying one letter at a time." << endl << endl;
}

/* Parameter is an empty Lexicon to store the words in the dictionary.
 * Opens file on user input, tests if file exists, and if so, populates Lexicon with
 * words from file.*/
void openFile(Lexicon& dict) {
   ifstream infile;
   string file = promptUserForFile(infile, "Dictionary file name: ", "Unable to open that file.  Try again.");
   dict.addWordsFromFile(file);
   infile.close();
}

/* Parameters are a string to store the first word in, a string to
 * store the second word in, and a populated Lexicon with words in the dictionary.
 * Checks if the words aren't in the dict, are the same, or aren't the same length.
 * Returns false if user presses enter.
 * Return true otherwise. */
bool getWords(string& word1, string& word2, const Lexicon& dict) {
    bool validWords = false;
    cout << endl;
    while (!validWords) {
        word1 = getLine("Word 1 (or Enter to quit): ");
        if (word1.empty()) {
            return false;
        }
        word2 = getLine("Word 2 (or Enter to quit): ");
        if (word2.empty()) {
            return false;
        }
        word1 = toLowerCase(word1);
        word2 = toLowerCase(word2);
        if (word1.length() != word2.length()){
            cout << "The two words must be the same length." << endl;
        } else if (word1 == word2) {
            cout << "The two words must be different." << endl;
        } else if (!dict.contains(word1) || !dict.contains(word2))  {
            cout << "The two words must be found in the dictionary." << endl;
        } else {
            validWords = true;
        }
    }
    return validWords;
}

/* Parameters include the first word, the second word, a Lexicon storing the dictionary,
 * and an empty Stack<string> to store the final word ladder in.
 * Creates partial ladders starting from word1 until shortest ladder to word 2 is found. */
void getLadder(string word1, string word2, const Lexicon& dict, Stack<string>& wordLadder) {
    Lexicon usedWords {word1};
    Queue<Stack<string> > laddersQueue = {{word1}};
    bool word2Found = false;
    while (!laddersQueue.isEmpty() && !word2Found) {
        Stack<string> ladder = laddersQueue.dequeue();
        for (int index = 0; index < ladder.peek().length(); index++) {
            for (char ch = 'a'; ch <= 'z'; ch++) {
                string newWord = ladder.peek();
                newWord[index] = ch;
                if (dict.contains(newWord)) {
                    if (!usedWords.contains(newWord)) {
                        usedWords.add(newWord);
                        Stack<string> newLadder = ladder;
                        newLadder.push(newWord);
                        laddersQueue.enqueue(newLadder);
                        if (newWord == word2) {
                            word2Found = true;
                            wordLadder = newLadder;
                        }
                    }
                }
            }
        }
    }
}

/* Parameters include the first word, the second word, and a Stack<string>
 * representing the word ladder.
 * Prints out the ladder a word at a time. */
void printLadder(string word1, string word2, Stack<string>& ladder) {
    cout << "A ladder from " + word2 + " back to " + word1 + ":" << endl;
    while (!ladder.isEmpty()) {
        cout << ladder.pop() + " ";
    }
    cout << endl << endl;
}

