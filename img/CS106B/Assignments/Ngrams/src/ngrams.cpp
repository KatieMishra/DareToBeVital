/* Katie Mishra, CS106B, Fall 2018
 * Ngrams takes in a file of sample text. Then using a number n provided by the user,
 * it creates a map of n-grams and the words that follow them. Uses map to generate
 * a given number of words of text to simulate the inputted file text. */

#include <iostream>
#include "console.h"
#include "vector.h"
#include "map.h"
#include "queue.h"
#include "simpio.h"
#include "filelib.h"
#include "random.h"
using namespace std;

void printWelcomeMessage();
void openFile(Queue<string>& wordsInFile);
void buildMap(Map<Queue<string>, Vector<string>>& map, Queue<string>& wordsInFile, int n);
bool getN(int& n, const Queue<string>& wordsInFile);
bool generateRandomText(Map<Queue<string>, Vector<string>>& map, const int& n, Queue<string>& text);
void printText(Queue<string>& text);

int main() {
    printWelcomeMessage();
    Queue<string> wordsInFile;
    openFile(wordsInFile);
    Map<Queue<string>, Vector<string>> map;
    int n = 1;
    if (getN(n, wordsInFile)) {
        buildMap(map, wordsInFile, n);
        Queue<string> text;
        while (generateRandomText(map, n, text)) {
            printText(text);
        }
    }
    cout << "Exiting." << endl;
    return 0;
}

//Prints welcome message
void printWelcomeMessage() {
    cout << "Welcome to CS 106B/X Random Writer ('N-Grams')!" << endl
         << "This program generates random text based on a document." << endl
         << "Give me an input file and an 'N' value for groups" << endl
         << "of words, and I'll create random text for you." << endl << endl;
}

/* Parameter is an empty Queue to store the individual words in the file.
 * Opens file on user input. */
void openFile(Queue<string>& wordsInFile) {
    ifstream infile;
    string file = promptUserForFile(infile, "Input file name? ", "Unable to open that file.  Try again.");
    string word;
    while (infile >> word) {
        wordsInFile.enqueue(word);
    }
    infile.close();
}

/* Parameters include length of n-grams and a populated queue of words in the file.
 * Gets value of n, which must be greater than 1 and less than the number of words in the file.
 * Returns false is n is greater than the num words in the file and true otherwise. */
bool getN(int& n, const Queue<string>& wordsInFile) {
    while (n < 2) {
        n = getInteger("Value of N? ", "Invalid format. Please enter an integer.");
    }
    if (n > wordsInFile.size()) {
        cout << "N is larger than the number of words in the file." << endl
             << "Unable to compute N-grams." << endl;
        return false;
    }
    return true;
}

/* Parameters include an empty map, a populated queue of words in the file, and the
 * length of the n-gram. */
void buildMap(Map<Queue<string>, Vector<string>>& map, Queue<string>& wordsInFile, int n) {
    Queue<string> firstNWords;
    Queue<string> window;
    for (int count = 0; count < 2; count++) {
        string nextWord = wordsInFile.dequeue();
        firstNWords.enqueue(nextWord);
        window.enqueue(nextWord);
    }
    while (!wordsInFile.isEmpty()) {
        string followingWord = wordsInFile.dequeue();
        if (map.containsKey(window)) {
            map[window].add(followingWord);
        } else {
            Vector<string> newKey {followingWord};
            map[window] = newKey;
        }
        window.dequeue();
        window.enqueue(followingWord);
    }
    while (!firstNWords.isEmpty()) {
        string followingWord = firstNWords.dequeue();
        if (map.containsKey(window)) {
            map[window].add(followingWord);
        } else {
            Vector<string> newKey {followingWord};
            map[window] = newKey;
        }
        window.dequeue();
        window.enqueue(followingWord);
    }
}

/* Parameters include a populated map of n-grams and their subsequent words, the length of the n-grams,
 * and an empty queue to store the random text in.
 * Gets the number of words to generate. Then randomly starts at an n-gram to loop over and generate
 * text.
 * Returns false if user enters 0 to quit game, true otherwise.  */
bool generateRandomText(Map<Queue<string>, Vector<string>>& map, const int& n, Queue<string>& text) {
    int numWords = -1;
    while (numWords != 0 && numWords < n) {
        cout << endl;
        numWords = getInteger("# of random words to generate (0 to quit): ", "Illegal numeric format.");
        if (numWords != 0 && numWords < n) {
            cout << "Must be at least " << n << " words." << endl;
        }
    }
    if (numWords != 0) {
        Queue<string> window = map.keys()[randomInteger(0, n-1)];

        while (text.size() < numWords) {
            window.enqueue(map.get(window)[randomInteger(0, map.get(window).size()-1)]);
            text.enqueue(window.dequeue());
        }
        return true;
    }
    return false;
}

/* Parameter is a populated queue with the generated text.
 * Prints text to screen.*/
void printText(Queue<string>& text) {
    cout << "...";
    while (!text.isEmpty()) {
        cout << " " << text.dequeue();
    }
    cout << " ..." << endl;
}

