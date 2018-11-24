/*
 * CS 106B/X
 * This file implements a general "basic" graphical user interface class.
 * See basicgui.h for declarations and documentation.
 *
 * @version 2018/11/10
 * - 18au 106B initial version
 *
 * @author Marty Stepp
 *
 * Known open issue: Leaks memory for all GObjects created.  Oops.
 */

#include "basicgui.h"
#include <iostream>
#include "error.h"
#include "gcolor.h"
#include "gevents.h"
#include "gfont.h"
#include "gthread.h"
#include "simpio.h"
#include "sound.h"
#include "strlib.h"

// declare/initialize member variables
BasicGUI::BasicGUI()
        : _window(nullptr),
          _promptLabel(nullptr),
          _statusLabel(nullptr),
          _textField(nullptr),
          _yesButton(nullptr),
          _noButton(nullptr),
          _consoleOutput(true),
          _graphicalOutput(true),
          _sound(true),
          _labelColor("#028040"),
          _labelColorError("#A22020") {
    // empty (uninitialized)
}

BasicGUI::~BasicGUI() {
    // TODO: delete
    _window = nullptr;
    _promptLabel = nullptr;
    _statusLabel = nullptr;
    _textField = nullptr;
    _yesButton = nullptr;
    _noButton = nullptr;
}

// implementation of static member functions
void BasicGUI::askNone() {
    _promptLabel->setText(" ");   // empty, but retain its height
    _yesButton->setVisible(false);
    _noButton->setVisible(false);
    _textField->setVisible(false);
    _window->repaint();
}

void BasicGUI::askText(const std::string& prompt, const std::string& initialValue) {
    GThread::runOnQtGuiThread([this, prompt, initialValue]() {
        std::string goodPrompt = trim(prompt);   // + " ";
        _promptLabel->setText(goodPrompt);
        if (_consoleOutput && !trim(prompt).empty()) {
            _lastPrompt = goodPrompt + " ";
            // std::cout << std::endl;   // to force flush
            // std::cout.flush();   // because of lack of endl, to see output right away
        }
        _yesButton->setVisible(false);
        _noButton->setVisible(false);
        _textField->setVisible(true);

        _yesButton->setAccelerator("");
        _noButton->setAccelerator("");
        _textField->setText(initialValue);
        _textField->setEnabled(true);
        _textField->requestFocus();
        _window->repaint();
    });
}

void BasicGUI::askYesOrNo(const std::string& prompt) {
    GThread::runOnQtGuiThread([this, prompt]() {
        std::string goodPrompt = trim(prompt);   // + " ";
        _promptLabel->setText(goodPrompt);
        if (_consoleOutput && !goodPrompt.empty()) {
            _lastPrompt = goodPrompt + " ";
            // std::cout << std::endl;   // to force flush
            // std::cout.flush();   // because of lack of endl, to see output right away
        }
        _yesButton->setEnabled(true);
        _noButton->setEnabled(true);
        _yesButton->setAccelerator("y");
        _noButton->setAccelerator("n");

        _yesButton->setVisible(true);
        _noButton->setVisible(true);
        _textField->setVisible(false);

        _yesButton->requestFocus();
        _window->repaint();
    });
}

void BasicGUI::close() {
    if (isInitialized()) {
        _window->setVisible(false);
        _window->close();
        // TODO: delete
        // delete _window;
        _window = nullptr;
        _statusLabel = nullptr;
    }
}

void BasicGUI::displayMessage(const std::string& message, bool force) {
    if (_consoleOutput && (force || !trim(message).empty())) {
        std::cout << message << std::endl;
    }
    _statusLabel->setText(trim(message));
}

void BasicGUI::ensureInitialized() {
    if (!isInitialized()) {
        error("GUI has not been initialized yet");
    }
}

void BasicGUI::handleNoClick() {
    // TODO: override me
}

void BasicGUI::handleTextFieldAnyKeyPress(GEvent /*event*/) {
    // TODO: override me
}

void BasicGUI::handleTextFieldEnterKeyPress() {
    // TODO: override me
}

void BasicGUI::handleYesClick() {
    // TODO: override me
}

void BasicGUI::preinitialize() {
    if (!isInitialized() && _graphicalOutput) {
        // create entire GUI
        _window = new GWindow(/* visible */ false);
        _window->setResizable(true);
        _window->setExitOnClose(true);

        _promptLabel = new GLabel("");
        _window->setRegionAlignment(GWindow::REGION_NORTH, ALIGN_RIGHT);
        _window->addToRegion(_promptLabel, GWindow::REGION_NORTH);
        _promptLabel->setVisible(false);

        // we kind of hack a horizontal padding into the buttons using HTML and nbsp
        _yesButton = new GButton("Yes");
        GFont::boldFont(_yesButton);
        _yesButton->setActionListener([this]() {
            // echo user input for logging and debugging
            if (_consoleOutput) {
                std::cout << _lastPrompt << "y" << std::endl;
                _lastPrompt = "";
            }
            handleYesClick();
        });
        _window->addToRegion(_yesButton, GWindow::REGION_NORTH);
        _yesButton->setVisible(false);

        _noButton = new GButton("No");
        GFont::boldFont(_noButton);
        _noButton->setActionListener([this]() {
            // echo user input for logging and debugging
            if (_consoleOutput) {
                std::cout << _lastPrompt << "n" << std::endl;
                _lastPrompt = "";
            }
            handleNoClick();
        });
        _window->addToRegion(_noButton, GWindow::REGION_NORTH);
        _noButton->setVisible(false);

        _textField = new GTextField(20);
        _textField->setActionListener([this]() {
            // echo user input for logging and debugging
            if (_consoleOutput) {
                std::cout << _lastPrompt << _textField->getText() << std::endl;
                _lastPrompt = "";
            }
            handleTextFieldEnterKeyPress();
        });
        _textField->setTextChangeListener([this](GEvent event) {
            handleTextFieldAnyKeyPress(event);
        });
        _window->addToRegion(_textField, GWindow::REGION_NORTH);
        _textField->setVisible(false);

        if (!_statusLabel) {
            // set up status label
            _statusLabel = new GLabel(" ");
            _window->addToRegion(_statusLabel, GWindow::REGION_SOUTH);
        }
    }
}

void BasicGUI::initialize() {
    GThread::runOnQtGuiThread([this]() {
        preinitialize();
        guiSetup();
        postinitialize();
    });
}

void BasicGUI::postinitialize() {
    if (_graphicalOutput) {
        if (!_window->isVisible()) {
            _window->center();
            _window->setVisible(true);
        }

        _textField->requestFocus();

        // clear out any in-progress game state
        reset();
    }
}

void BasicGUI::guiSetup() {
    // TODO in subclass: perform any subclass-specific initialization
}

bool BasicGUI::isInitialized() const {
    return _window != nullptr;
}

void BasicGUI::playSound(const std::string& filename) {
    if (_sound) {
        Sound::playSound(filename);
    }
}

void BasicGUI::reset() {
    ensureInitialized();

    // clear out the counts of words recorded so far and scores
    setStatusMessage("Initializing ...");
    setStatusMessage("");
}

void BasicGUI::setConsoleOutput(bool output) {
    _consoleOutput = output;
}

void BasicGUI::setGraphicalOutput(bool output) {
    _graphicalOutput = output;
}

void BasicGUI::setSound(bool sound) {
    _sound = sound;
}

void BasicGUI::setWidgetBackground(int color) {
    _textField->setBackground(color);
    _yesButton->setBackground(color);
    _noButton->setBackground(color);
}

void BasicGUI::setWidgetColor(int color) {
    _promptLabel->setColor(color);
    _statusLabel->setColor(color);
    _textField->setColor(color);
    _yesButton->setColor(color);
    _noButton->setColor(color);
    _labelColor = GColor::convertRGBToColor(color);
}

void BasicGUI::setWidgetFont(const std::string& font) {
    _promptLabel->setFont(font);
    _statusLabel->setFont(font);
    _textField->setFont(font);
    _yesButton->setFont(font);
    _noButton->setFont(font);
}

void BasicGUI::setWidgetFontSize(int size) {
    // TODO: scale for high-DPI screens
    std::string fontStr = "*-*-" + std::to_string(size);
    _promptLabel->setFont(fontStr);
    _statusLabel->setFont(fontStr);
    _textField->setFont(fontStr);
    _yesButton->setFont(fontStr);
    _noButton->setFont(fontStr);
}

void BasicGUI::setStatusMessage(const std::string& status, bool isError) {
    ensureInitialized();
    _statusLabel->setColor(isError ? _labelColorError : _labelColor);
    _statusLabel->setLabel(status);
    // if (_consoleOutput && !status.empty()) {
    //     std::cout << status << std::endl;
    // }
}

void BasicGUI::shutdown() {
    if (isInitialized()) {
        close();
    }
}


GraphicalUI::GraphicalUI(BasicGUI* gui)
        : _gui(gui) {
    // empty
}

GraphicalUI::~GraphicalUI() {
    // TODO: delete
    _gui = nullptr;
}

void GraphicalUI::feedInput(bool b) {
    _lock.lockForWrite();
    _bools.enqueue(b);
    _lock.unlock();
}

void GraphicalUI::feedInput(const std::string& s) {
    _lock.lockForWrite();
    _strings.enqueue(s);
    _lock.unlock();
}

void GraphicalUI::print(const std::string& message) {
    _gui->displayMessage(message);
}

std::string GraphicalUI::readLine(const std::string& prompt, const std::string& initialValue) {
    _gui->askText(prompt, initialValue);
    std::string result;

    // wait for a string to be deposited in the queue
    while (true) {
        _lock.lockForRead();
        bool empty = _strings.isEmpty();
        if (!empty) {
            result = _strings.dequeue();
            _lock.unlock();
            break;
        } else {
            _lock.unlock();
        }
        GThread::sleep(10);
    }
    return result;
}

bool GraphicalUI::readBoolean(const std::string& prompt) {
    _gui->askYesOrNo(prompt);
    bool result;

    // wait for a boolean value to be deposited in the queue
    while (true) {
        _lock.lockForRead();
        bool empty = _bools.isEmpty();
        if (!empty) {
            result = _bools.dequeue();
            _lock.unlock();
            break;
        } else {
            _lock.unlock();
        }
        GThread::sleep(10);
    }
    return result;
}
