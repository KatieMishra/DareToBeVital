/* Katie Mishra -- Fall Quarter 2018 -- CS106B
 * This program draws two types of fractals -- Sierpinski Triangles and Trees. 
 * ADD MORE DESCRIPTION
 */

#include "fractals.h"
#include "gwindow.h"
using namespace std;

void drawSingleTriangle(GWindow& window, double x, double y, double size);
void drawBranches(GWindow& gw, double x, double y, double size);

//avoid redrawing lines
void drawSierpinskiTriangle(GWindow& window, double x, double y, double size, int order) {
    if (order >= 1) {
        drawSingleTriangle(window, x, y, size);
        order--;
        drawSierpinskiTriangle(window, x, y, size/2, order);
        drawSierpinskiTriangle(window, x+size/2, y, size/2, order);
        drawSierpinskiTriangle(window, x+size/4, y+(size/4*sqrt(3)), size/2, order);
    } else if (order < 0) {
        throw std::invalid_argument("received negative value");
    }
}

/* Parameters -- window: GUI window to draw figure on
 *               x: top left x cordinate of triangle
 *               y: top left y cordinate of triangle
 *               size: length of triangle's sides
 * Draws a triangle at the given coordinates  */
void drawSingleTriangle(GWindow& window, double x, double y, double size) {
    window.drawPolarLine(x,y,size,0);
    window.drawPolarLine(x,y,size,-60);
    window.drawPolarLine(x+size,y,size,-120);
}

/* Parameters -- gw: GUI window to draw figure on
 *               x: top left x cordinate of bounding box
 *               y: top left y cordinate of bounding box
 *               size: length of branches
 *               order: how many levels of branches to draw
 * Draws a tree starting with the single base and then with 7 branches
 * at each node. Draws as many levels of branches as order dictates. */
void drawTree(GWindow& gw, double x, double y, double size, int order) {
    if (order >= 2) {
        gw.setColor("#8b7765");
        gw.drawLine(x+size/2,y+size/2,x+size/2,y+size);
        drawBranches(gw, x, y, size/2);
        drawTree(gw, x, y, size/2, order--);
    } else if (order == 1) {
        gw.setColor("#2e8b57");
        gw.drawLine(x+size/2,y+size/2,x+size/2,y+size);
        drawBranches(gw, x, y, size);
    } else if (order < 0) {
        throw std::invalid_argument("received negative value");
    }
}

/* Parameters -- gw: GUI window to draw figure on
 *               x: top left x cordinate of bounding box
 *               y: top left y cordinate of bounding box
 *               size: length of branches
 * Draws 6 branches from differing angles  */
void drawBranches(GWindow& gw, double x, double y, double size) {
    int degree = 30;
    for (int numBranches = 0; numBranches < 6; numBranches++) {
        degree += 15;
        gw.drawPolarLine(x+size/2, y+size/2, size, 30);
    }
}
