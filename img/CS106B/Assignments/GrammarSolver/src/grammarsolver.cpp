/* Katie Mishra & Akshaya Dinesh -- CS106B -- Fall 2018
 * Grammar Solver recursively generates expansions of non-terminals given
 * a set of grammar rules. */

#include "grammarsolver.h"
#include "map.h"
#include "random.h"
#include "vector.h"
using namespace std;

void loadFileIntoMap(Map<string, Vector<Vector<string> > >& grammar, istream& input);
Vector<string> generateExpansions(Map<string, Vector<Vector<string> > >& grammar, int times, string& symbol);
string generateExpansionsRecursively(Map<string, Vector<Vector<string> > >& grammar, string symbol);

/* Parameters -- input: object to handle file reading
 *               symbol: string of which non-terminal to generate
 *               times: number of times to generate the given non-terminal
 * Program takes in a BNF description of a language and loads it into a map from
 * non-terminals to terminals. From a user inputted non-terminal, recursively
 * expands until function returns a string of terminals. Generates as many
 * expansions as the user asks for.
 * Return -- Vector<string> of the expansions of the given symbol
 */
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    //throw error if symbol parameter is empty
    if (symbol == "") {
        throw "symbol parameter is empty";
    }
    Map<string, Vector<Vector<string> > > grammar;
    loadFileIntoMap(grammar, input);
    Vector<string> expansions = generateExpansions(grammar, times, symbol);
    return expansions;
}

/* Parameters -- grammar: reference to an empty map of string:vector(vector(string)) to
 *               store the grammar in
 *               input: istream to open and read file
 * Opens, reads, and loops over file. Tokenizes each line and stores to map from non-terminal
 * to terminals. */
void loadFileIntoMap(Map<string, Vector<Vector<string> > >& grammar, istream& input) {
    string line;
    while (getline(input, line)) {
        string nonTerminal = trim(line.substr(0,stringIndexOf(line,"::=")));
        //throw exception if 2+ lines for same non-terminal
        if (grammar.containsKey(nonTerminal)) {
            throw "grammar contains more than one line for the same non-terminal";
        }

        line = line.substr(stringIndexOf(line,"=")+1,line.length()-1);
        Vector<string> tokenizedLine = stringSplit(line,"|");

        //Vector stores separated rules in the line, tokenized by spaces
        Vector<Vector<string> > allRulesInLine;
        /* Creates a vector for each rule in the line, and then stores each of those
         * smaller vectors in a larger vector to represent all the rules for that line */
        for (int rule = 0; rule < tokenizedLine.size(); rule++) {
            Vector<string> ruleVector;
            Vector<string> tokenizedRule = stringSplit(tokenizedLine[rule]," ");
            for (int terminal = 0; terminal < tokenizedRule.size(); terminal++) {
                if (tokenizedRule[terminal] != " ") {
                    ruleVector.add(tokenizedRule[terminal]);
                }
            }
            allRulesInLine.add(ruleVector);
        }

        grammar.put(nonTerminal, allRulesInLine);
    }
}

/* Parameters -- grammar: reference to a populated map of string:vector(vector(string)) which stores
 *               the grammar
 *               times: number of expansions to generate
 * Calls recursive function to generate expansions according to the number of expansions the user wants.
 * Stores expansions in a Vector.
 * Return -- Vector<string> of the expansions generated.*/
Vector<string> generateExpansions(Map<string, Vector<Vector<string> > >& grammar, int times, string& symbol) {
    Vector<string> expansions;
    for (int i = 0; i < times; i++) {
        //If symbol isn't a non-terminal in grammar, add it as its own expansion
        if (!grammar.containsKey(symbol)) {
            expansions.add(symbol);
          //else do recursion to generate expansion
        } else {
            string expansion = generateExpansionsRecursively(grammar, symbol);
            //remove extra space at end of expansion
            expansion = expansion.substr(0,expansion.length()-1);
            expansions.add(expansion);
        }
    }
    return expansions;
}

/* Parameters -- grammar: reference to a populated map of string:vector(vector(string)) which stores
 *               the grammar
 *               symbol: reference to the symbol to generate an expansion for
 *               expansion: reference to string to store expansion in
 * Recursively chooses a random terminal for a given terminal, and returns a terminal if it can't be
 * expanded further.
 * Return -- string of the generated expansion. */
string generateExpansionsRecursively(Map<string, Vector<Vector<string> > >& grammar, string symbol) {
    if (!grammar.containsKey(symbol)) {
        return symbol + " ";
    }
    string expansion = "";
    Vector<string> rule = randomElement(grammar.get(symbol));
    for (int i = 0; i < rule.size(); i++) {
        expansion.append(generateExpansionsRecursively(grammar, rule[i]));
    }
    return expansion;
}
