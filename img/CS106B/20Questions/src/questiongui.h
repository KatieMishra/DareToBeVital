/*
 * CS 106B/X 20 Questions
 * This file declares the graphical user interface for 20 Questions.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @version 2018/11/10
 * - 18au 106B initial version
 *
 * @author Marty Stepp
 */

#ifndef _questiongui_h
#define _questiongui_h

#include "basicgui.h"
#include "questiontree.h"

class QuestionGUI : public BasicGUI {
public:
    /**
     * Returns the singleton GUI object.
     */
    static QuestionGUI* instance();

protected:
    enum State {
        INITIAL,
        MAIN_MENU,
        PLAYING_GAME,
        MERGING,
        WIN,
        LOSE
    };

    /**
     * Creates a new GUI.  Private to prevent unwanted construction.
     */
    QuestionGUI();

    /**
     * Frees up memory allocated by the GUI.
     */
    virtual ~QuestionGUI() Q_DECL_OVERRIDE;

    /**
     * Initializes the GUI.
     */
    virtual void guiSetup() Q_DECL_OVERRIDE;

    /**
     * Functions to handle events on various widgets.
     */
    virtual void handleNoClick() Q_DECL_OVERRIDE;
    virtual void handleTextFieldAnyKeyPress(GEvent event) Q_DECL_OVERRIDE;
    virtual void handleTextFieldEnterKeyPress() Q_DECL_OVERRIDE;
    virtual void handleYesClick() Q_DECL_OVERRIDE;

    /**
     * Displays graphical main menu.
     */
    virtual void mainMenu(bool sound = true);

    /**
     * Initiates merging the tree data with the data in the given file.
     */
    virtual void mergeData(std::string filename);

    /**
     * Begins the playing of a question game.
     */
    virtual void playGame();

    /**
     * Possibly plays a sound with a given probability.
     */
    virtual void playSoundMaybe(const std::string& kind = "");

    /**
     * Initiates reading the tree data from the given file.
     */
    virtual void readData(std::string filename);

    /**
     * Displays statistics about the tree.
     */
    virtual void stats();

    /**
     * Initiates writing the tree data to the given file.
     */
    virtual void writeData(std::string filename);

    /*
     * Constants
     * ---------
     * To configure fonts, sizing, etc.
     */
    static const int LABEL_COLOR;
    static const int LABEL_FONT_SIZE;
    static const bool ITS_A_TRAP;
    static const bool SOUND_ENABLED;
    static const double SOUND_PERCENTAGE;
    static const std::string RESOURCE_FILE_NAME_PREFIX;
    static const std::string BACKGROUND_IMAGE;
    static const std::string INPUT_FILE_NAME_TEXT;
    static const std::string MAIN_MENU_TEXT;
    static const std::string MEMORY_FILE;
    static const std::string OUTPUT_FILE_NAME_TEXT;
    static const std::string RECALL_MEMORY_TEXT;
    static const std::string WINDOW_TITLE;

    // pointer to the singleton GUI object
    static QuestionGUI* _instance;

    GraphicalUI* _ui;
    QuestionTree* _questionTree;
    State _state;
    GImage* _backgroundImage;

    // collections of sound wav files that can be played
    Vector<std::string> _soundFiles;
    Vector<std::string> _soundFilesPlayGame;
    Vector<std::string> _soundFilesMainMenu;
    Vector<std::string> _soundFilesWin;
    Vector<std::string> _soundFilesLose;

    friend int main();   // me and main, we're kind of a big deal
};

#endif // _questiongui_h
