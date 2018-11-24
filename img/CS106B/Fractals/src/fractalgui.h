/*
 * CS 106B/X Fractal Recursion Problems
 * This instructor-provided file declares a class representing the overall
 * graphical user interface (GUI) for this part of the program.
 * See fractalgui.cpp for the implementation of each member.
 *
 * You don't need to modify this file.
 * Your recursive functions should work with an unmodified version of this file.
 *
 * @author Marty Stepp
 * @version 2018/10/11
 * - 18au 106B initial version;
 *   added H-tree (but disabled);
 *   new Qt-based GUI;
 *   crosshairs on drag rectangle;
 *   2px-thick Sierpinski lines; etc.
 * @version 2018/01/25
 * - disabled Mandelbrot set for CS 106B version
 * @version 2017/10/14
 * - modified Complex parse logic to use default on failure
 *   (addresses unusual parsing failures on some systems)
 * @version 2017/10/12
 * - initial 17au version;
 *   modified with additional fractals (Mandelbrot Set) based on cgregg 17wi/sp version
 * - added spinner to show while graphics are drawing
 * @version 2016/10/16
 * - modified for 16au with additional fractals (tree)
 * @version 2015/04/24
 * - modified for 15sp version, including exception-handling
 * @version 2014/10/13
 * - modified for 14au version, including new version of floodFill
 */

#ifndef _fractalgui_h
#define _fractalgui_h

// uncomment to indicate which fractals are enabled
#define SIERPINSKI_ENABLED 1
#define FLOOD_FILL_ENABLED 1
//#define H_TREE_ENABLED 1
//#define TREE_ENABLED 1
//#define MANDELBROT_SET_ENABLED 1
//#define LOADING_LABEL_ENABLED 1


#ifdef MANDELBROT_SET_ENABLED
#include "complex.h"
#endif // MANDELBROT_SET_ENABLED
#include <string>
#include "map.h"
#include "ginteractors.h"
#include "gobjects.h"
#include "gwindow.h"

class FractalGUI {
public:
    FractalGUI();

    virtual ~FractalGUI();

    /*
     * Removes all shapes from the screen.
     */
    void clear();

    /*
     * Creates and configures all graphical controls in the GUI.
     */
    void initialize();

    static FractalGUI* instance();

private:
    /*
     * Initiates a flood fill operation on the window.
     */
    void doFloodFill(double x, double y);

    /*
     * Initiates a recursive H-tree operation on the window.
     */
#ifdef H_TREE_ENABLED
    void doHTree();
#endif // H_TREE_ENABLED

#ifdef MANDELBROT_SET_ENABLED
    /*
     * Initiates a Mandelbrot Set operation on the window.
     */
    void doMandelbrot();
#endif // MANDELBROT_SET_ENABLED

    /*
     * Initiates a draw Sierpinski triangle operation on the window.
     */
    void doSierpinski();

    /*
     * Initiates a recursive tree operation on the window.
     */
#ifdef TREE_ENABLED
    void doTree();
#endif // TREE_ENABLED

    /*
     * Adds a bunch of rectangles to the screen for use with the floodFill function.
     */
    void floodFill_addShapes();

    /*
     * Returns a map of available colors for flood fill.
     */
    const Map<std::string, int>& getColorMap();

    /*
     * Returns true if all text fields have valid integer coordinates entered into them.
     */
    bool hasAllCoords();

#ifdef MANDELBROT_SET_ENABLED
    /*
     * Tries to read a complex number range for Mandelbrot Set
     * and store it in the given two Complex number objects.
     * Returns true if the GUI should proceed with drawing the set.
     */
    bool readComplexRange(Complex& min, Complex& max);
#endif // MANDELBROT_SET_ENABLED

    /*
     * Scales the fractal based on zoom factor and where user clicked.
     */
    void scale(int x, int y, double zoomFactor);

    /*
     * Sets the bottom status bar text to display in the southern area.
     * If isError is true, shows it in a red error color.
     */
    void setStatusText(const std::string& text, bool isError = false);

    // constants
    static const int WINDOW_WIDTH;
    static const int WINDOW_HEIGHT;
    static const int FLOOD_FILL_NUM_SHAPES;
    static const int FLOOD_FILL_MIN_RECT_SIZE;
    static const int FLOOD_FILL_MAX_RECT_SIZE;
    static const int DRAG_RECT_COLOR;
    static const int MIN_ORDER;
    static const int MAX_ORDER;

    // to 'rig' random number generator; set to <= 0 to disable
    static const int FLOOD_FILL_RANDOM_SEED = 106;

    // overall window
    GWindow* window;

    // west buttons to draw fractals
    GButton* sierpinskiButton;
    GButton* htreeButton;
    GButton* treeButton;
    GButton* mandelbrotButton;
    GButton* floodFillButton;
    GButton* floodFillRectsButton;
    GButton* diffButton;
    GButton* clearButton;

    // flood fill / Mandelbrot colors drop-down
    GChooser* colorChooser;

    // north text fields for figure parameters
    GTextField* xField;
    GTextField* yField;
    GTextField* sizeField;
    GTextField* orderField;
    GCheckBox* animationBox;
    GLabel* loadingLabel;

    // south status bar
    GLabel* statusBar;

    // temporary dragging rectangle for figure size
    GRect* dragRect;
    GLine* dragLineH;
    GLine* dragLineV;

    // temporary flags to remember if we are in midst of a flood fill/Mandelbrot
    bool fillMode;
    bool fillInProgress = false;

    // last complex number range typed for Mandelbrot Set
    std::string lastComplexRange;

    static FractalGUI* _instance;
};

#endif // _fractalgui_h
