/*
 * CS 106B/X, Marty Stepp
 * This program is a demonstration of the Stanford library's BasicGraph class.
 * We write a method 'coolest' that reads a file of fake Twitter data
 * and returns which user has the most followers-of-followers,
 * using a graph as in intermediate data structure to help us.
 */

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include "console.h"
#include "strlib.h"
#include "basicgraph.h"
using namespace std;

// function prototype declarations
string raddest(istream& input);

int main() {
    ifstream input;
    input.open("twitter.txt");

    string coolPerson = raddest(input);
    cout << "The coolest person is " << coolPerson << endl;

    return 0;
}

/*
 * Returns the person with the most second-level followers in the given graph.
 * The input file contains lines of the form:
 * "Stuart Marty" to indicate that Stuart follows Marty.
 */
string raddest(istream& input) {
    // read the file data into a graph
    // - each vertex is a person
    // - each edge B -> A indicates that A follows B
    //   (we reverse the edges because we want to process followers-of-followers,
    //    and inverting the social graph direction makes this easier)
    BasicGraph graph;
    string name1, name2;
    while (input >> name1 >> name2) {
        graph.addVertex(name1);
        graph.addVertex(name2);
        graph.addEdge(name2, name1);
    }

    // figure out raddest person (most friends-of-friends)
    string raddest;
    int raddestCount = 0;
    for (Vertex* person : graph.getVertexSet()) {
        // count the number of friends-of-friends of this person
        int count = 0;
        for (Vertex* eachFriend : graph.getNeighbors(person)) {
            count += graph.getNeighbors(eachFriend).size();
        }
        if (count > raddestCount) {
            raddestCount = count;
            raddest = person->name;
        }
    }

    return raddest;
}
