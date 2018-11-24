/* Katie Mishra & Akshaya Dinesh -- Fall 2018 -- CS106B
 * This program recursively prints, in alphabetical order, all the anagrams for a given phrase. An anagram is a word
 * or phrase made by rearranging the letters of another word or phrase. Words in the anagram can only be from a
 * provided dictionary.
 */

#include <string>
#include "letterinventory.h"
#include "set.h"
#include "lexicon.h"
#include "foreach.h"
using namespace std;

void generatePossibleWords(LetterInventory& li, const Set<string>& dictionary, Vector<string>& possibleWords);
void findAnagramsHelper(Vector<string>& chosenWords, Vector<string>& possibleWords, LetterInventory& li, int& counter, int& max);

/**
 * @brief findAnagrams: finds all rearrangements of of the letters of a given
 *                      phrase using words in a provided dictionary.
 * @param phrase: string to generate anagrams for
 * @param max: upper threshold of number of separate words to have in anagram
 * @param dictionary: all possible words that appear in phrase
 * @return number of anagrams found for phrase given max number of words in each
 */
int findAnagrams(string phrase, int max, Set<string>& dictionary) {
    int counter = 0;
    if (max < 0) {
        throw max;
    } else {
        Vector<string> possibleWords;
        Vector<string> chosenWords;
        LetterInventory li(phrase);

        generatePossibleWords(li, dictionary, possibleWords);
        findAnagramsHelper(chosenWords, possibleWords, li, counter, max);
    }
    return counter;
}

/**
 * @brief generatePossibleWords: loops over words in dictionary, and if li has all the letters to make a word,
 *                               adds the word to a Vector of possible words
 * @param li: all the letters in the provided phrase
 * @param dictionary: defines all valid words
 * @param possibleWords: vector to store all valid words from li and dictionary
 */
void generatePossibleWords(LetterInventory& li, const Set<string>& dictionary, Vector<string>& possibleWords) {
    for (string word: dictionary) {
        if (li.contains(word)) {
            possibleWords.add(word);
        }
    }
}

/**
 * @brief findAnagramsHelper: loops over all words in the possible words list. Explores partial trees starting with each
 *                            word via recursive backtracking. Keeps a counter of all anagrams found of length max or less
 *                            that use up all letters in li.
 * @param chosenWords: words already used in the anagram
 * @param possibleWords: all words that can possibly be used in the anagram
 * @param li: all letters that are in the given phrase, and have not yet been used in the anagram
 * @param counter: number of anagrams of length max found
 * @param max: max number of words in anagram
 */
void findAnagramsHelper(Vector<string>& chosenWords, Vector<string>& possibleWords, LetterInventory& li, int& counter, int& max) {
    if (li.isEmpty()) {
        //base case -- no letters left
        cout << chosenWords << endl;
        counter++;
    } else {
        // recursive case: try each word
        for (string word : possibleWords) {
            /*if li has all letters to make the word and chosenWords is less than the max length, start
             *into recursive backtracking*/
            if (li.contains(word) && (chosenWords.size() < max || max == 0)) {
                //choose
                li.subtract(word);
                chosenWords.add(word);

                //explore
                findAnagramsHelper(chosenWords, possibleWords, li, counter, max);

                //unchoose
                li.add(word);
                chosenWords.removeBack();
            }
        }
    }
}
