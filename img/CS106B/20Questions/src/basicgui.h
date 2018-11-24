/*
 * CS 106B/X
 * This file declares a general "basic" graphical user interface class.
 * It provides a basic layout with buttons in the north for user interaction
 * including asking yes/no questions (bool) and open-ended questions (string).
 * The center region can be used to present application-specific graphics or
 * widgets.  The south region contains a status label that can be updated.
 *
 * This class is meant to be extended by a GUI subclass for particular assignments.
 * For example, this serves as a nice template for the Boggle GUI or for the
 * GUI for the 20 Questions binary tree game.
 *
 * @version 2018/11/10
 * - 18au 106B initial version
 *
 * @author Marty Stepp
 */

#ifndef _basicgui_h
#define _basicgui_h

#include <QReadWriteLock>
#include <string>
#include "gbutton.h"
#include "gevent.h"
#include "glabel.h"
#include "gobjects.h"
#include "gtextfield.h"
#include "gtypes.h"
#include "gwindow.h"
#include "ui.h"

class BasicGUI {
public:
    /**
     * Frees up memory allocated by the GUI.
     */
    virtual ~BasicGUI();

    /**
     * Removes any prompt for input from GUI.
     */
    virtual void askNone();

    /**
     * Prompts user to type text into text field.
     */
    virtual void askText(const std::string& prompt, const std::string& initialValue = "");

    /**
     * Prompts user to choose Yes or No by clicking Y/N buttons.
     */
    virtual void askYesOrNo(const std::string& prompt);

    /**
     * Closes and discards the graphical window.
     */
    virtual void close();

    /**
     * Displays the given output message on the GUI.
     * Omits blank messages/lines unless 'force' is true (default false).
     */
    virtual void displayMessage(const std::string& message, bool force = false);

    /**
     * Internal helper to make sure GUI is initialized.
     * Throws an ErrorException if the GUI has not been initialized.
     */
    virtual void ensureInitialized();

    /**
     * Functions to handle events on various widgets.
     */
    virtual void handleNoClick() /* = 0 */;
    virtual void handleTextFieldAnyKeyPress(GEvent event) /* = 0 */;
    virtual void handleTextFieldEnterKeyPress() /* = 0 */;
    virtual void handleYesClick() /* = 0 */;

    /**
     * Initializes the GUI as it is created.
     */
    virtual void preinitialize();
    virtual void initialize();
    virtual void postinitialize();

    /**
     * Returns true if the GUI has been initialized.
     */
    virtual bool isInitialized() const;

    /**
     * Plays a sound effect located in the given file.
     * (NOTE: Sound playing is broken in the current version of the Stanford C++
     *  back-end library, so you probably should not use this function.  Sorry.)
     */
    virtual void playSound(const std::string& filename);

    /**
     * Clears the GUI's state.
     */
    virtual void reset();

    /**
     * Sets whether the GUI will produce console output (default true).
     */
    virtual void setConsoleOutput(bool output);

    /**
     * Sets whether the GUI will produce graphical output (default true).
     */
    virtual void setGraphicalOutput(bool output);

    /**
     * Sets whether the GUI will play sound if requested (default true).
     */
    virtual void setSound(bool sound);

    /**
     * Sets various colors and fonts used on some interactors in the GUI.
     */
    virtual void setWidgetBackground(int color);
    virtual void setWidgetColor(int color);
    virtual void setWidgetFont(const std::string& font);
    virtual void setWidgetFontSize(int size);

    /**
     * Sets the GUI window to display the given text as a status message.
     * If the 'isError' flag is true, will be displayed as an error in red.
     */
    virtual void setStatusMessage(const std::string& status, bool isError = false);

    /**
     * Closes the GUI window and frees all associated memory.
     */
    virtual void shutdown();

protected:
    /**
     * Creates a new GUI.  Private to prevent unwanted construction.
     */
    BasicGUI();

    /**
     * Initializes the GUI as it is created.
     * You must override this method in the subclass to perform any
     * subclass-specific initialization.
     */
    virtual void guiSetup() = 0;   // override in subclasses

    // pointers to graphical objects used in the graphics window
    GWindow* _window;
    GLabel* _promptLabel;
    GLabel* _statusLabel;
    GTextField* _textField;
    GButton* _yesButton;
    GButton* _noButton;
    bool _consoleOutput;
    bool _graphicalOutput;
    bool _sound;
    std::string _labelColor;
    std::string _labelColorError;
    std::string _lastPrompt;
};

class GraphicalUI : public UI {
public:
    GraphicalUI(BasicGUI* gui);
    virtual ~GraphicalUI() Q_DECL_OVERRIDE;
    virtual void print(const std::string& message) Q_DECL_OVERRIDE;
    virtual std::string readLine(const std::string& prompt = "",
                                 const std::string& initialValue = "") Q_DECL_OVERRIDE;
    virtual bool readBoolean(const std::string& prompt = "") Q_DECL_OVERRIDE;

    virtual void feedInput(bool b);
    virtual void feedInput(const std::string& s);

private:
    BasicGUI* _gui;
    Queue<bool> _bools;
    Queue<std::string> _strings;
    QReadWriteLock _lock;    // avoid thread race conditions
};

#endif // _basicgui_h
