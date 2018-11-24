/*
 * CS 106B/X Tiles
 * This file contains the declaration of the TileNode structure.
 *
 * Please DO NOT MODIFY this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @author Marty Stepp
 * @version 2016/11/03
 * - 16au 106X initial version
 */

#ifndef _tilegui_h
#define _tilegui_h

#include <iostream>
#include <string>
#include "gbutton.h"
#include "gevent.h"
#include "glabel.h"
#include "gobjects.h"
#include "gradiobutton.h"
#include "gtextfield.h"
#include "gwindow.h"
#include "tilelist.h"

class TileGUI {
public:
    // constants
    // set this to false for the same graphical output every time
    static const bool RANDOM = true;

    /*
     * Frees the memory used by the GUI.
     */
    virtual ~TileGUI();

    /*
     * Creates one or more new tiles with a random x/y position,
     * width, height, and color, and adds it to the given tile list.
     */
    void addRandomTiles(bool toFront = true);

private:
    /*
     * Constructs the graphical window and widgets on the screen.
     */
    TileGUI();

    void clearAndRedraw();
    void processMouseEvent(GEvent& mouseEvent);

    GWindow* _window;
    GButton* _gbAddFront;
    GButton* _gbAddBack;
    GButton* _gbClear;
    GButton* _gbDebug;
    GRadioButton* _grbMove;
    GRadioButton* _grbHighlight;
    GRadioButton* _grbColor;
    GRadioButton* _grbRaise;
    GRadioButton* _grbLower;
    GRadioButton* _grbRemove;
    GRadioButton* _grbRemoveAll;
    GTextField* _gtfAddNumber;
    GLabel* _glMousePos;

    // list of all tiles (student's code)
    TileList* _tileList;

    // information for dragging a tile to new x/y position
    TileNode* _dragTile;
    int _dragX;
    int _dragY;

    // constants
    static const int WINDOW_WIDTH = 700;
    static const int WINDOW_HEIGHT = 500;
    static const int MIN_SIZE = 50;
    static const int MAX_SIZE = 150;
    static const int MIN_COLOR = 50;
    static const int MAX_COLOR = 255;
    static const int TILE_COUNT = 1;

    // Sam : Frodo :: main : TileGUI
    friend int main();
};

#endif // _tilegui_h
