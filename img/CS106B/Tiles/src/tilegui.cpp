/*
 * CS 106B/X Tiles
 * This GUI uses your TileList class and contains the 'main'
 * function to set up the overall program's graphical user interface.
 * Some commands print debug messages that appear on the bottom console.
 *
 * Please DO NOT MODIFY this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * Extra feature -- can select change color radio button and, if a valid tile is
 * selected, change it to a random color.
 *
 * @author Marty Stepp
 * @version 2017/10/30
 * - 17au 106X initial version
 * @version 2016/11/03
 * - 16au 106X initial version
 */

#include "tilegui.h"
#include <iomanip>
#include "gthread.h"
#include "random.h"
#include "stack.h"
#include "strlib.h"

TileGUI::TileGUI() {
    // possibly use the same random numbers every time for testing
    if (!TileGUI::RANDOM) {
        setRandomSeed(106);
    }

    _window = new GWindow(WINDOW_WIDTH, WINDOW_HEIGHT, /* visible */ false);
    _window->center();
    _window->setWindowTitle("CS 106B Tiles");
    _window->setRepaintImmediately(false);
    _window->setExitOnClose(true);
    _window->setMouseListener([this](GEvent event) {
        processMouseEvent(event);
    });
    _window->setWindowListener([this](GEvent event) {
        if (_window->isVisible() && event.getEventType() == WINDOW_RESIZED) {
            clearAndRedraw();
        }
    });

    _gbAddFront = new GButton("Add Front");
    _gbAddFront->setActionCommand("addFront");
    _gbAddFront->setAccelerator("ctrl F");
    _gbAddFront->setActionListener([this]() {
        addRandomTiles(/* toFront */ true);
    });

    _gbAddBack = new GButton("Add Back");
    _gbAddBack->setActionCommand("addBack");
    _gbAddBack->setAccelerator("ctrl B");
    _gbAddBack->setActionListener([this]() {
        addRandomTiles(/* toFront */ false);
    });

    _gbClear = new GButton("Clear");
    _gbClear->setActionCommand("clear");
    _gbClear->setActionListener([this]() {
        std::cout << "clear" << std::endl;
        _tileList->clear();
        clearAndRedraw();
    });

    _gbDebug = new GButton("Debug");
    _gbDebug->setActionCommand("debug");
    _gbDebug->setActionListener([this]() {
        std::cout << "debug" << std::endl;
        _tileList->debug();
        clearAndRedraw();
    });

    _grbMove = new GRadioButton("Move");
    _grbMove->setActionCommand("move");
    _grbMove->setAccelerator("ctrl M");

    _grbHighlight = new GRadioButton("Highlight");
    _grbHighlight->setActionCommand("highlight");
    _grbHighlight->setAccelerator("ctrl H");
    _grbHighlight->setSelected(true);

    _grbRaise = new GRadioButton("Raise");
    _grbRaise->setActionCommand("raise");
    _grbRaise->setAccelerator("ctrl R");

    _grbLower = new GRadioButton("Lower");
    _grbLower->setActionCommand("lower");
    _grbLower->setAccelerator("ctrl L");

    _grbRemove = new GRadioButton("Remove");
    _grbRemove->setActionCommand("remove");
    _grbRemove->setAccelerator("ctrl M");

    _grbRemoveAll = new GRadioButton("Remove All");
    _grbRemoveAll->setActionCommand("removeAll");
    _grbRemoveAll->setAccelerator("ctrl V");

    _grbColor = new GRadioButton("Change Color");
    _grbColor->setActionCommand("color");
    _grbColor->setAccelerator("ctrl O");

    _gtfAddNumber = new GTextField(/* value */ TILE_COUNT, /* min */ 1, /* max */ 999);
    _gtfAddNumber->requestFocus();

    _glMousePos = new GLabel("(x=___, y=___)");

    // layout
    _window->addToRegion(_grbHighlight, "North");
    _window->addToRegion(_grbColor, "North");
    _window->addToRegion(_grbRaise, "North");
    _window->addToRegion(_grbLower, "North");
    _window->addToRegion(_grbRemove, "North");
    _window->addToRegion(_grbRemoveAll, "North");
    _window->addToRegion(_gbAddFront, "South");
    _window->addToRegion(_gbAddBack, "South");
    _window->addToRegion(_gtfAddNumber, "South");
    _window->addToRegion(_gbClear, "South");
    _window->addToRegion(_glMousePos, "South");
    _window->addToRegion(_gbDebug, "South");

    _window->setVisible(true);

    _tileList = new TileList();
    _dragTile = nullptr;
    _dragX = 0;
    _dragY = 0;
}

// commenting out due to some kind of GUI memory bug; not a big deal
TileGUI::~TileGUI() {
//    delete gbAddFront;
//    delete gbAddBack;
//    delete gbClear;
//    delete gbDebug;
//    delete grbHighlight;
//    delete grbRaise;
//    delete grbLower;
//    delete grbRemove;
//    delete grbRemoveAll;
//    delete glMousePos;
//    delete window;
}

void TileGUI::addRandomTiles(bool toFront) {
    if (!_gtfAddNumber->valueIsInteger()) {
        return;
    }

    int count = _gtfAddNumber->getValueAsInteger();
    for (int i = 0; i < count; i++) {
        // pick random coords and size
        int width = randomInteger(MIN_SIZE, MAX_SIZE);
        int height = randomInteger(MIN_SIZE, MAX_SIZE);
        int x = randomInteger(0, WINDOW_WIDTH - width);
        int y = randomInteger(0, WINDOW_HEIGHT - height);

        // pick random color
        int r = randomInteger(MIN_COLOR, MAX_COLOR);
        int g = randomInteger(MIN_COLOR, MAX_COLOR);
        int b = randomInteger(MIN_COLOR, MAX_COLOR);
        std::ostringstream colorout;
        colorout << "#" << std::setbase(16) << std::setfill('0') << r << g << b;
        std::string color = colorout.str();

        // add to top of tile list
        std::cout << (toFront ? "addFront" : "addBack")
                  << " (x=" << x << ", y=" << y << ", w=" << width
                  << ", h=" << height << ", color=\"" << color << "\")" << std::endl;
        TileNode* node = new TileNode(x, y, width, height, color);
        if (toFront) {
            // call the student's addFront function
            _tileList->addFront(node);
        } else {
            // call the student's addBack function
            _tileList->addBack(node);
        }
    }

    clearAndRedraw();
}

void TileGUI::clearAndRedraw() {
    _window->clearCanvas();
    std::cout << "drawAll" << std::endl;

    // draw the tiles (in reverse order)
    Stack<TileNode*> stack;

    // call the student's getFront function
    for (TileNode* curr = _tileList->getFront(); curr; curr = curr->next) {
        stack.push(curr);
    }
    while (!stack.isEmpty()) {
        TileNode* curr = stack.pop();
        curr->draw(*_window);
    }

    _window->repaint();
}

//bool TileGUI::processKeyEvent(GKeyEvent& keyEvent, TileList& tlist) {
//    int type = keyEvent.getEventType();
//    if (type == KEY_PRESSED) {
//        if (tolower(keyEvent.getKeyChar()) == 'a') {
//            // listen for 'A' to add a new random tile
//            addRandomTiles(tlist, /* count */ 1,
//                           /* toFront */ !keyEvent.isShiftKeyDown());
//            return true;
//        } else if (tolower(keyEvent.getKeyChar()) == 'c') {
//            // listen for 'C' to clear all tiles
//            tlist.clear();
//            return true;
//        }
//    }
//    return false;
//}

void TileGUI::processMouseEvent(GEvent& mouseEvent) {
    int x = static_cast<int>(mouseEvent.getX());
    int y = static_cast<int>(mouseEvent.getY());
    int type = mouseEvent.getEventType();
    if (type == MOUSE_PRESSED) {
        // call the student's highlight, raise, lower, remove, or removeAll function
        if (_grbHighlight->isSelected()) {
            std::cout << "highlight (x=" << x << ", y=" << y << ")" << std::endl;
            _tileList->changeColor(x, y, "yellow");
        } else if (_grbRaise->isSelected()) {
            std::cout << "raise (x=" << x << ", y=" << y << ")" << std::endl;
            if (_tileList->raise(x, y)) {
                // initiate dragging of the front tile
                _dragTile = _tileList->getFront();
                if (_dragTile) {
                    _dragX = x;
                    _dragY = y;
                }
            }
         /* If the change color radio button is selected, and a
          * valid tile is clicked, changes to a random color.*/
        } else if (_grbColor->isSelected()) {
            // pick random color
            int r = randomInteger(MIN_COLOR, MAX_COLOR);
            int g = randomInteger(MIN_COLOR, MAX_COLOR);
            int b = randomInteger(MIN_COLOR, MAX_COLOR);
            std::ostringstream colorout;
            colorout << "#" << std::setbase(16) << std::setfill('0') << r << g << b;
            std::string color = colorout.str();

            std::cout << "highlight (x=" << x << ", y=" << y << ")" << std::endl;
            _tileList->changeColor(x, y, color);
        } else if (_grbLower->isSelected()) {
            std::cout << "lower (x=" << x << ", y=" << y << ")" << std::endl;
            _tileList->lower(x, y);
        } else if (_grbRemove->isSelected()) {
            std::cout << "remove (x=" << x << ", y=" << y << ")" << std::endl;
            _tileList->remove(x, y);
        } else if (_grbRemoveAll->isSelected()) {
            std::cout << "removeAll (x=" << x << ", y=" << y << ")" << std::endl;
            _tileList->removeAll(x, y);
        }
        clearAndRedraw();
    } else if (type == MOUSE_MOVED) {
        std::ostringstream out;
        out << "(x=" << std::setw(3) << std::setfill('0') << x << ", y="
            << std::setw(3) << std::setfill('0') << y << ")";
        _glMousePos->setText(out.str());
    } else if (type == MOUSE_DRAGGED) {
        if (_dragTile) {
            // move tile based on difference in mouse position
            _dragTile->x += x - _dragX;
            _dragTile->y += y - _dragY;
            _dragX = x;
            _dragY = y;
            clearAndRedraw();
        }
    } else if (type == MOUSE_RELEASED) {
        _dragTile = nullptr;
        _dragX = 0;
        _dragY = 0;
    }
}


// main function for overall program
int main() {
    // set up GUI and widgets
    /*TileGUI* gui =*/ new TileGUI();
    return 0;
}
