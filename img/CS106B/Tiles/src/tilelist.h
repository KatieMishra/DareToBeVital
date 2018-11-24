/* Katie Mishra & Akshaya Dinesh -- CS106B -- Fall 2018
 * Defines header file for Tiles program, and all the functions
 * that can be called. */

#ifndef _tilelist_h
#define _tilelist_h

#include <iostream>
#include <string>
#include "tilenode.h"
using namespace std;

class TileList {
public:
    TileList();
    ~TileList();
    void addFront(TileNode* node);
    void addBack(TileNode* node);
    void clear();
    void debug();
    TileNode* getFront() const;
    void changeColor(int x, int y, string color);
    void lower(int x, int y);
    bool raise(int x, int y);
    TileNode* remove(int x, int y);
    int removeAll(int x, int y);

private:
    TileNode* front;
};

#endif // _tilelist_h
