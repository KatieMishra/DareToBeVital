/* Katie Mishra -- CS106B -- Fall 2018
 * Lets the user play 20 questions, where they can load in files of
 * set question/answers. Computer can become smarter over time as user can
 * input new questions/answers to expand the binary search tree. Also tracks
 * number of games won/lost, and the number of answer/question nodes.
 * Extra feature -- lets computer ask a max of 20 questions before it automatically loses
 *                  code for extra feature in getAnswerNode function starting on line 171
 */

#include "questiontree.h"
#include <climits>
#include "strlib.h"
#include "random.h"
#include "simpio.h"
#include "filelib.h"
using namespace std;

/**
 * @brief QuestionTree::QuestionTree -- constructor initalizes first tree node to be "A:computer"
 */
QuestionTree::QuestionTree() {
    tree = new QuestionNode();
    tree->data = "A:computer";
    numGamesWon = 0;
    numGamesLost = 0;
    numQuestionsAsked = 0;
}

/**
 * @brief QuestionTree::~QuestionTree -- destructor clears the tree
 */
QuestionTree::~QuestionTree() {
    QuestionNode* temp = tree;
    clear(temp);
    delete temp;
}

/**
 * @brief QuestionTree::clear -- recursively frees all memory in tree
 * @param front -- first node in tree
 */
void QuestionTree::clear(QuestionNode*& front) {
    if (front->yes == nullptr) {
        delete front;
    } else {
        clear(front->no);
        clear(front->yes);
    }
}

/**
 * @brief QuestionTree::countAnswers -- counts how many answers nodes
 * there are in the tree
 * @return -- num of answer nodes in tree
 */
int QuestionTree::countAnswers() const {
   return countAnswersHelper(tree);
}

/**
 * @brief countAnswersHelper -- recursively counts num answer nodes in tree
 * @param currNode -- node currently being tested in the tree
 * @return -- num answer nodes in tree
 */
int QuestionTree::countAnswersHelper(QuestionNode* const &currNode) const {
    //base case, answer node found
    if ((currNode->data)[0] == 'A') {
        return 1;
    //base case, empty tree
    } else if (currNode->yes == nullptr) {
        return 0;
    //recursive case, count num answer nodes off of yes and no nodes
    } else {
        return countAnswersHelper(currNode->yes) + countAnswersHelper(currNode->no);
    }
}

/**
 * @brief QuestionTree::countQuestions -- counts num question nodes in tree
 * @return -- num question nodes
 */
int QuestionTree::countQuestions() const {
    return countQuestionsHelper(tree);
}

/**
 * @brief countQuestionsHelper -- recursively counts num question nodes in tree
 * @param currNode -- node currently being tested in the tree
 * @return -- num question nodes
 */
int QuestionTree::countQuestionsHelper(QuestionNode* const &currNode) const {
    //base case, empty tree or hit an answer node
    if (currNode->yes == nullptr) {
        return 0;
    } else {
        return 1 + countQuestionsHelper(currNode->yes) + countQuestionsHelper(currNode->no);
    }
}

/**
 * @brief QuestionTree::gamesLost -- counts num games lost over user's session
 * @return -- num games lost
 */
int QuestionTree::gamesLost() const {
    return numGamesLost;
}

/**
 * @brief QuestionTree::gamesWon -- counts num games lost over user's session
 * @return -- num games won
 */
int QuestionTree::gamesWon() const {
    return numGamesWon;
}

/**
 * @brief QuestionTree::mergeData -- takes in a file from user, and puts existing data on left / right side
 * of tree, based on a new question to distinguish the data
 * @param ui -- user interface object to read / write to console
 * @param input -- file reader to open user's input file
 */
void QuestionTree::mergeData(UI& ui, std::istream& input) {
    string question = ui.readLine("Type a Y/N question to distinguish my data from this file: ");
    bool answer = ui.readBoolean("And what is the answer for the new data? ");
    //store current question or answer in the first node
    QuestionNode* temp = new QuestionNode(tree->data);
    temp->yes = tree->yes;
    temp->no = tree->no;
    tree->data = "Q:" + question;
    string line;
    QuestionNode* newNode = new QuestionNode();
    //determine which side of the tree the old / new data goes on based on answer to question
    tree->no = answer ? temp : newNode;
    tree->yes = answer ? newNode : temp;
    mergeDataHelper(input, line, newNode);
}

/**
 * @brief writeDataToNode -- recursively writes input data to given side of the tree
 * @param input -- file reader to open user's input file
 * @param currNode -- node to start writing new data to
 */
void QuestionTree::mergeDataHelper(std::istream& input, string line, QuestionNode*& currNode) {
    if (getline(input, line)) {
        currNode->data = line;
        if (line[0] == 'Q') {
            currNode->yes = new QuestionNode();
            currNode->no = new QuestionNode();
            readDataHelper(input,line,currNode->yes);
            readDataHelper(input,line,currNode->no);
        }
    }
}

/**
 * @brief QuestionTree::playGame -- plays game by asking questions to reach an answer node and return whether that node
 * matches the user's object
 * @param ui -- object to read / write to the console
 * @return -- return bool as to whether computer won
 */
bool QuestionTree::playGame(UI& ui) {
    numQuestionsAsked = 0;
    return getAnswerNode(ui,tree);
}

/**
 * @brief getAnswerNode -- recurisvely traverses through tree, asking questions and moving
 * left or right accordingly, until hitting an answer node
 * @param answerNode -- current node to test for an answer node
 */
bool QuestionTree::getAnswerNode(UI& ui, QuestionNode*& currNode) {
    if (numQuestionsAsked == 20) {
        ui.print("Darn it! I already asked 20 questions, so I automatically lose :(");
        return false;
    }
    //base case -- answer node
    if (currNode->yes == nullptr) {
        string oldAnswer = currNode->data.substr(2);
        bool correctGuess = ui.readBoolean("Are you thinking of: " + oldAnswer + "? ");
        if (correctGuess) {
            ui.print("Horray, I win!");
            numGamesWon++;
        } else {
            numGamesLost++;
            //get and store new question / answer pair to distinguish from incorrect answer
            string correctAnswer = ui.readLine("Drat, I lost. What was your object? ");
            string question = ui.readLine("Type a Y/N question to distinguish " + correctAnswer + " from " + oldAnswer + ": ");
            bool answerToQuestion = ui.readBoolean("And what is the answer for " + correctAnswer + "? ");
            currNode->data = "Q:" + question;
            currNode->yes = new QuestionNode("A:" + (answerToQuestion ? correctAnswer : oldAnswer));
            currNode->no = new QuestionNode("A:" + (answerToQuestion ? oldAnswer : correctAnswer));
        }
        return correctGuess;
    //recursive case -- question node
    } else {
        string question = currNode->data.substr(2);
        numQuestionsAsked++;
        return (ui.readBoolean(question)) ? getAnswerNode(ui, currNode->yes) : getAnswerNode(ui, currNode->no);
    }
    return false;
}

/**
 * @brief QuestionTree::answerRange -- finds mix and max answer nodes in tree
 * @param minDepth -- minimum depth answer node
 * @param maxDepth -- maximum depth answer node
 */
void QuestionTree::answerRange(int& minDepth, int& maxDepth) const {
    int currDepth = 1;
    minDepth = 0;
    maxDepth = 0;
    answerRangeHelper(minDepth, maxDepth, tree, currDepth);
}

/**
 * @brief QuestionTree::answerRangeHelper -- recursively finds min and max answer nodes in tree
 * @param minDepth -- minimum depth answer node
 * @param maxDepth -- maximum depth answer node
 * @param currNode -- current node being examined
 * @param currDepth -- current depth of the tree being examined
 */
void QuestionTree::answerRangeHelper(int& minDepth, int& maxDepth, QuestionNode* const &currNode, int& currDepth) const {
    //base case, answer node found
    if ((currNode->data)[0] == 'A') {
        if (minDepth == 0) {
            minDepth = currDepth;
        }
        if (currDepth > maxDepth) {
            maxDepth = currDepth;
        }
    //recursive case, recursively go through yes and no nodes
    } else {
        currDepth += 1;
        answerRangeHelper(minDepth, maxDepth, currNode->yes, currDepth);
        answerRangeHelper(minDepth, maxDepth, currNode->no, currDepth);
    }
}

/**
 * @brief QuestionTree::readData -- takes in a file name and reads data into a binary tree
 * of questions / answers
 * @param input -- file reader
 */
void QuestionTree::readData(std::istream& input) {
    QuestionNode* currNode = tree;
    if (tree->yes != nullptr) clear(tree);
    string line;
    readDataHelper(input, line, currNode);
}

/**
 * @brief QuestionTree::readDataHelper -- given file name, recursively reads data into a binary tree
 * of questions / answers
 * @param input -- file reader
 * @param line -- current line of text to read in
 * @param currNode -- current node to add question / answer to
 */
void QuestionTree::readDataHelper(std::istream& input, string line, QuestionNode*& currNode) {
    //if file still has more lines
    if (getline(input, line)) {
        currNode->data = line;
        //if line is a question, add it and the nodes below it
        if (line[0] == 'Q') {
            currNode->yes = new QuestionNode();
            currNode->no = new QuestionNode();
            readDataHelper(input,line,currNode->yes);
            readDataHelper(input,line,currNode->no);
        //if line is an answer, add it and backtrack
        } else {
            return;
        }
    }
}

/**
 * @brief QuestionTree::writeData -- takes current binary tree data and writes it to a new file
 * @param output -- file writer
 */
void QuestionTree::writeData(std::ostream& output) {
    writeDataHelper(output,tree);
}

/**
 * @brief QuestionTree::writeDataHelper -- recursively writes current binary tree to a new file
 * @param output -- output object writer
 * @param currNode -- current node being examined
 */
void QuestionTree::writeDataHelper(std::ostream& output, QuestionNode*& currNode) {
    output << currNode->data << endl;
    if (currNode->yes != nullptr) {
        writeDataHelper(output, currNode->yes);
        writeDataHelper(output,currNode->no);
    }
}
