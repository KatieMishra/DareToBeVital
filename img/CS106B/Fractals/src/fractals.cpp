// This is the CPP file you will edit and turn in. (TODO: Remove this comment!)
#include "fractals.h"
using namespace std;

double getHeight(double size) {
    return 0.5 * size * sqrt(3.0);
}

void drawSierpinskiTriangle(GWindow& window, double x, double y, double size, int order) {
    if(order < 0) {
        throw "invalid input";
    } else if(order == 0) {
        return;
    } else if(order == 1) {
        window.drawLine(x, y, x + size, y);
        window.drawLine(x, y, x + size / 2, y + getHeight(size));
        window.drawLine(x + size, y, x + size / 2, y + getHeight(size));
    } else {
        drawSierpinskiTriangle(window, x, y, size / 2, order - 1);
        drawSierpinskiTriangle(window, x + size / 2,  y, size / 2, order - 1);
        drawSierpinskiTriangle(window, x + size / 4, y + getHeight(size) / 2, size / 2, order - 1);
    }
}

int floodFillHelper(GWindow& window, int x, int y, int& newColor, int& oldColor) {
    if(!window.inCanvasBounds(x, y) || window.getPixel(x, y) != oldColor) {
        return 0;
    } else {
        window.setPixel(x, y, newColor);
        return 1 + floodFillHelper(window, x + 1, y, newColor, oldColor)
            + floodFillHelper(window, x - 1, y, newColor, oldColor)
            + floodFillHelper(window, x, y + 1, newColor, oldColor)
            + floodFillHelper(window, x, y - 1, newColor, oldColor);
    }
}

int floodFill(GWindow& window, int x, int y, int color) {
    if(x > window.getCanvasWidth() || y > window.getCanvasHeight() || x < 0 || y < 0) {
        throw "invalid input";
    } else {
        int oldColor = window.getPixel(x, y);
        if(oldColor == color) {
            return 0;
        } else {
            return floodFillHelper(window, x, y, color, oldColor);
        }
    }
}
