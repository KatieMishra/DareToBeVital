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

#ifndef _questionconstants_h
#define _questionconstants_h

#include <string>

/**
 * A class of shared constants for use by 20 Questions main programs.
 */
class QuestionConstants {
public:
    static const bool USE_MEMORY_FILE;
    static const std::string FILE_IO_ERROR_MESSAGE;
    static const std::string INPUT_FILE_NAME_PROMPT;
    static const std::string INTRO_MESSAGE;
    static const std::string LOAD_MEMORY_MESSAGE;
    static const std::string MAIN_MENU_TEXT;
    static const std::string MEMORY_FILE;
    static const std::string OUTPUT_FILE_NAME_PROMPT;
    static const std::string OUTRO_MESSAGE;
    static const std::string READ_TREE_MESSAGE;
    static const std::string SAVE_MEMORY_MESSAGE;
    static const std::string WRITE_TREE_MESSAGE;

    /**
     * Produces and returns a string such as "Brain: Q0/A1, Games: W0/L0"
     * about the current game's question tree and games won/lost.
     */
    static std::string statsString(int questions, int answers,
                                   int minDepth, int maxDepth,
                                   int won, int lost);

private:
    QuestionConstants();   // prevent construction
};

#endif // _questionconstants_h
