/* Katie Mishra -- CS106B -- Fall 2018
 * Header file for 20 questions game.
 */

#ifndef _questiontree_h
#define _questiontree_h

#include <iostream>
#include <string>
#include "questionnode.h"
#include "ui.h"

class QuestionTree {
public:
    QuestionTree();
    ~QuestionTree();
    void answerRange(int& minDepth, int& maxDepth) const;
    int countAnswers() const;
    int countQuestions() const;
    int gamesLost() const;
    int gamesWon() const;
    void mergeData(UI& ui, std::istream& input);
    bool playGame(UI& ui);
    void readData(std::istream& input);
    void writeData(std::ostream& output);
    void clear(QuestionNode*& front);
    void writeDataToNode(std::istream& input, QuestionNode*& currNode);  

private:
    QuestionNode* tree;
    int numQuestionsAsked;
    int numGamesWon;
    int numGamesLost;
    void writeDataHelper(std::ostream& output, QuestionNode*& currNode);
    void mergeDataHelper(std::istream& input, std::string line, QuestionNode*& currNode);
    void readDataHelper(std::istream& input, std::string line, QuestionNode*& currNode);
    int countQuestionsHelper(QuestionNode* const &currNode) const;
    int countAnswersHelper(QuestionNode* const &currNode) const;
    bool getAnswerNode(UI& ui, QuestionNode*& currNode);
    void answerRangeHelper(int& minDepth, int& maxDepth, QuestionNode* const &currNode, int& currDepth) const;
};

#endif // _questiontree_h
