/*
 * CS 106B/X 20 Questions
 * This file implements the graphical user interface for 20 Questions.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @version 2018/11/10
 * - initial version for 106B 18au
 *
 * @author Marty Stepp (18au)
 *
 * Known open issue: Leaks memory for all GObjects created.  Oops.
 */

#include "questiongui.h"
#include "questionconstants.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include "error.h"
#include "filelib.h"
#include "gcolor.h"
#include "gfilechooser.h"
#include "gthread.h"
#include "random.h"

// declare/initialize static variables and constants
/*static*/ const int QuestionGUI::LABEL_COLOR = GColor::convertRGBToRGB(6, 226, 240);
/*static*/ const int QuestionGUI::LABEL_FONT_SIZE = 12;
/*static*/ const bool QuestionGUI::ITS_A_TRAP = false;
/*static*/ const bool QuestionGUI::SOUND_ENABLED = true;
/*static*/ const double QuestionGUI::SOUND_PERCENTAGE = 0.6;
/*static*/ const std::string QuestionGUI::RESOURCE_FILE_NAME_PREFIX =
        QuestionGUI::ITS_A_TRAP ? "ackbar" : "vader";
/*static*/ const std::string QuestionGUI::BACKGROUND_IMAGE =
        RESOURCE_FILE_NAME_PREFIX + ".png";
/*static*/ const std::string QuestionGUI::WINDOW_TITLE = "CS 106B 20 Questions";

/*static*/ QuestionGUI* QuestionGUI::_instance = nullptr;

QuestionGUI::QuestionGUI()
        : BasicGUI(),
          _state(INITIAL) {
    // empty
}

QuestionGUI::~QuestionGUI() {
    delete _questionTree;
    _questionTree = nullptr;

    // TODO: delete
    _backgroundImage = nullptr;
}

void QuestionGUI::handleNoClick() {
    std::string text = _promptLabel->getText();
    if (text == QuestionConstants::LOAD_MEMORY_MESSAGE) {
        // don't load from file
        // start the main menu
        displayMessage("", /* force */ true);
        mainMenu();
    } else {
        // feed the Yes click to the GraphicalUI wrapper for student to read
        _ui->feedInput(false);
    }
    playSoundMaybe();
}

void QuestionGUI::handleTextFieldAnyKeyPress(GEvent /*event*/) {
    std::string prompt = _promptLabel->getText();
    std::string text = trim(_textField->getText());
    if (prompt == QuestionConstants::MAIN_MENU_TEXT) {
        if (text.length() == 1) {
            // immediately respond to the key press; don't wait for them to press Enter
            if (_consoleOutput) {
                std::cout << _lastPrompt << text << std::endl;
                _lastPrompt = "";
            }
            handleTextFieldEnterKeyPress();
        }
    }
}

void QuestionGUI::handleTextFieldEnterKeyPress() {
    // feed the typed string to the GraphicalUI wrapper for student to read
    std::string prompt = _promptLabel->getText();
    std::string text = _textField->getText();
    _textField->setText("");
    if (prompt == QuestionConstants::MAIN_MENU_TEXT) {
        // user has chosen an option from the main menu
        std::string choice = toUpperCase(trim(text));
        if (choice == "P") {
            playGame();
        } else if (choice == "R") {
            askText(QuestionConstants::INPUT_FILE_NAME_PROMPT);
        } else if (choice == "W") {
            askText(QuestionConstants::OUTPUT_FILE_NAME_PROMPT);
        } else if (choice == "M") {
            _state = MERGING;
            askText(QuestionConstants::INPUT_FILE_NAME_PROMPT);
        } else if (choice == "S") {
            stats();
        } else if (choice == "Q") {
            displayMessage("", /* force */ true);
            if (QuestionConstants::USE_MEMORY_FILE) {
                displayMessage(QuestionConstants::SAVE_MEMORY_MESSAGE);
                writeData(QuestionConstants::MEMORY_FILE);
            }
            displayMessage(QuestionConstants::OUTRO_MESSAGE);
            close();
        } else {
            // re-prompt the main menu again
            displayMessage("", /* force */ true);
            mainMenu();
        }
    } else if (prompt == QuestionConstants::INPUT_FILE_NAME_PROMPT) {
        // user has typed an input file name to read
        if (_consoleOutput) {
            std::cout << _lastPrompt << text << std::endl;
            _lastPrompt = "";
        }
        if (_state == MERGING) {
            mergeData(text);
        } else {
            readData(text);
        }
    } else if (prompt == QuestionConstants::OUTPUT_FILE_NAME_PROMPT) {
        // user has typed an output file name to write
        if (_consoleOutput) {
            std::cout << _lastPrompt << text << std::endl;
            _lastPrompt = "";
        }
        writeData(text);
    } else {
        _ui->feedInput(text);
    }
}

void QuestionGUI::handleYesClick() {
    std::string prompt = _promptLabel->getText();
    if (prompt == QuestionConstants::LOAD_MEMORY_MESSAGE) {
        // load from file
        if (fileExists(QuestionConstants::MEMORY_FILE)) {
            displayMessage(QuestionConstants::READ_TREE_MESSAGE);
            std::ifstream input;
            input.open(QuestionConstants::MEMORY_FILE.c_str());
            _questionTree->readData(input);
            input.close();
        }

        // go to the main menu
        displayMessage("", /* force */ true);
        mainMenu();
    } else {
        // feed the Yes click to the GraphicalUI wrapper for student to read
        _ui->feedInput(true);
    }
    playSoundMaybe();
}


void QuestionGUI::guiSetup() {
    _window->setBackground(GColor::BLACK);
    _backgroundImage = new GImage(BACKGROUND_IMAGE);
    _window->add(_backgroundImage);
    _window->setResizable(false);
    _window->setTitle(WINDOW_TITLE);
    _window->setCanvasSize(_backgroundImage->getWidth(),
                           _backgroundImage->getHeight());

    setWidgetBackground(GColor::BLACK);
    setWidgetColor(LABEL_COLOR);
    setWidgetFontSize(LABEL_FONT_SIZE);

    _ui = new GraphicalUI(this);
    _questionTree = new QuestionTree();

    // read sound files from disk
    if (_soundFiles.isEmpty()) {
        for (std::string filename : listDirectory(".")) {
            if (startsWith(filename, RESOURCE_FILE_NAME_PREFIX) && endsWith(filename, ".wav")) {
                _soundFiles.add(filename);
                if (stringContains(filename, "-lose")) {
                    _soundFilesLose.add(filename);
                }
                if (stringContains(filename, "-play")) {
                    _soundFilesPlayGame.add(filename);
                }
                if (stringContains(filename, "-menu")) {
                    _soundFilesMainMenu.add(filename);
                }
                if (stringContains(filename, "-win")) {
                    _soundFilesWin.add(filename);
                }
            }
        }
    }

    // background music (disabled; cannot play > 1 audio clip at same time)
    // playSound("imperial-march.mp3");

    displayMessage(QuestionConstants::INTRO_MESSAGE);

    // set up game to ask to load from memory
    askYesOrNo(QuestionConstants::LOAD_MEMORY_MESSAGE);
}

/*static*/ QuestionGUI* QuestionGUI::instance() {
    if (!_instance) {
        _instance = new QuestionGUI();
    }
    return _instance;
}

void QuestionGUI::mainMenu(bool sound) {
    _state = MAIN_MENU;
    if (sound) {
        playSoundMaybe("menu");
    }
    askText(QuestionConstants::MAIN_MENU_TEXT);
}

void QuestionGUI::mergeData(std::string filename) {
    if (filename.empty()) {
        // browse for a file if none specified
        filename = GFileChooser::showOpenDialog();
    }
    if (filename.empty()) {
        return;
    }
    displayMessage(QuestionConstants::READ_TREE_MESSAGE);

    GThread::runInNewThreadAsync([this, filename]() {
        std::ifstream input;
        input.open(filename.c_str());
        if (input.fail()) {
            displayMessage(QuestionConstants::FILE_IO_ERROR_MESSAGE + filename);
        } else {
            _questionTree->mergeData(*_ui, input);
            input.close();
        }

        // return to main menu
        mainMenu();
    });
}

void QuestionGUI::playGame() {
    askNone();
    _state = PLAYING_GAME;
    playSoundMaybe("play");

    GThread::runInNewThreadAsync([this]() {
        // this call blocks until entire playGame logic has completed/returned
        bool result = _questionTree->playGame(*_ui);
        _state = result ? WIN : LOSE;
        playSoundMaybe();
        displayMessage("", /* force */ true);

        // once game is done playing, go back to main menu
        mainMenu(/* sound */ false);
    });
}

void QuestionGUI::playSoundMaybe(const std::string& kind) {
    if (!SOUND_ENABLED || !randomChance(SOUND_PERCENTAGE)) {
        // don't play a sound this time
        return;
    }

    Vector<std::string>& sounds =
            (kind == "menu" || _state == MAIN_MENU)    ? _soundFilesMainMenu :
            (kind == "play" || _state == PLAYING_GAME) ? _soundFilesPlayGame :
            (kind == "win"  || _state == WIN)          ? _soundFilesWin :
            (kind == "lose" || _state == LOSE)         ? _soundFilesLose :
                                                         _soundFiles;
    if (sounds.isEmpty()) {
        return;
    }
    std::string soundFile = randomElement(sounds);
    playSound(soundFile);
}

void QuestionGUI::readData(std::string filename) {
    if (filename.empty()) {
        // browse for a file if none specified
        filename = GFileChooser::showOpenDialog();
    }
    if (filename.empty()) {
        return;
    }
    displayMessage(QuestionConstants::READ_TREE_MESSAGE);
    std::ifstream input;
    input.open(filename.c_str());
    if (input.fail()) {
        displayMessage(QuestionConstants::FILE_IO_ERROR_MESSAGE + filename);
    } else {
        _questionTree->readData(input);
        input.close();
    }

    // return to main menu
    mainMenu();
}

void QuestionGUI::stats() {
    int questions = _questionTree->countQuestions();
    int answers   = _questionTree->countAnswers();
    int won = _questionTree->gamesWon();
    int lost = _questionTree->gamesLost();
    int minDepth = 0;
    int maxDepth = 0;
    _questionTree->answerRange(minDepth, maxDepth);
    displayMessage(QuestionConstants::statsString(
                       questions, answers,
                       minDepth, maxDepth,
                       won, lost) + "\n");

    // return to main menu
    mainMenu();
}

void QuestionGUI::writeData(std::string filename) {
    if (filename.empty()) {
        // browse for a file if none specified
        filename = GFileChooser::showSaveDialog();
    }
    if (filename.empty()) {
        return;
    }
    displayMessage(QuestionConstants::WRITE_TREE_MESSAGE);
    std::ofstream output;
    output.open(filename.c_str());
    if (output.fail()) {
        displayMessage(QuestionConstants::FILE_IO_ERROR_MESSAGE + filename);
    } else {
        _questionTree->writeData(output);
        output.close();
    }

    // return to main menu
    mainMenu();
}
