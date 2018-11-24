/* Katie Mishra & Akshaya Dinesh -- Fall 2018 -- CS106B
 * TileList lets users manipulate tiles on a screen, using pointers.
 * Users can highlight, remove, add, change-z-ordering, and clear
 * all tiles. */

#include "tilelist.h"
#include "random.h"

/* Initializes the TileList by creating front to be a nullptr.
 * Runs in O(1).*/
TileList::TileList() {
    front = nullptr;
}

/* Destroys the TileList by clearing out all the elements of the list.
 * Runs in O(N).*/
TileList::~TileList() {
    clear();
}

/* Adds given node to back of list by looping through the list and adding to the end.
 * Calls addFront if the front is null.
 * Runs in O(N).
 * Params -- newNode: node to add to back of list */
void TileList::addBack(TileNode* newNode) {
    if(front == nullptr) {
        addFront(newNode);
        return;
    }
    TileNode* temp = front;
    while (temp->next != nullptr) {
        temp = temp->next;
    }
    temp->next = newNode;
}

/* Adds a given node to the front of the list.
 * Runs in O(1).
 * Params -- newNode: node to add */
void TileList::addFront(TileNode* newNode) {
    newNode->next = front;
    front = newNode;
}

/* Loops through the list and prints to console.
 * Runs in O(N). */
void TileList::debug() {
    TileNode* temp = front;
    if(temp == nullptr) {
        cout << "null front" << endl;
        return;
    }
    while(temp->next != nullptr) {
        cout << temp->toString() << endl;
        temp = temp->next;
    }
    if(temp != nullptr) cout << temp->toString() << endl;
}

/* Returns node at front of list.
 * Runs in O(1).*/
TileNode* TileList::getFront() const {
    return front;
}

/* If user clicks on valid tile, makes that tile
 * yellow.
 * Runs in O(N).
 * Params -- x: top x coordinate of the tile
 *           y: left y coordinate of the tile */
void TileList::changeColor(int x, int y, string color) {
    TileNode* nodeToHighlight = front;
    if(nodeToHighlight == nullptr) {
        return;
    }
    //check all tiles from first to tile before last
    while (nodeToHighlight->next != nullptr) {
        if (nodeToHighlight->contains(x,y)) {
            nodeToHighlight->color=color;
            break;
        }
        nodeToHighlight = nodeToHighlight->next;
    }
    //check last tile
    if (nodeToHighlight->contains(x,y)) {
        nodeToHighlight->color=color;
    }
}

/* Lowers clicked tile to bottom of x-ordering.
 * Does so by looping through the list, finding the topmost tile.
 * Then it continues to loop and places it at the end.
 * Runs in O(N).
 * Params -- x: top x coordinate of the tile
 *           y: left y coordinate of the tile */
void TileList::lower(int x, int y) {
    bool foundNode = false;
    TileNode* nodeToMove = nullptr;
    TileNode* temp = front;

    // check if front was clicked
    if(front->contains(x, y)) {
        if(front == nullptr || front->next == nullptr) {
            return;
        }
        nodeToMove = front;
        front = front->next;
        foundNode = true;
    }

    /*check tiles from first to tile before last, until a tile is found */
    while (temp->next != nullptr) {
        if (temp->next->contains(x,y) && !foundNode) {
            if(temp->next->next == nullptr) {
                return;
            }
            foundNode = true;
            nodeToMove = temp->next;
            temp->next = nodeToMove->next;
        }
        temp = temp->next;
    }
    nodeToMove->next = nullptr;
    temp->next = nodeToMove;
}

/* If raise button is selected and the user clicks
 * on a valid tile, moves that tile to front of list.
 * Loops through list until tile is found, then adds it to the front of the list.
 * Runs in O(N).
 * Params -- x: top x coordinate of the tile
 *           y: left y coordinate of the tile
 * Return -- bool value of whether tile was successfully raised. */
bool TileList::raise(int x, int y) {
    bool foundNode = false;
    TileNode* nodeToMove = nullptr;
    TileNode* temp = front;

    // check if front was clicked
    if(front == nullptr || front->next == nullptr || front->contains(x, y)) {
        return true;
    }
    // check tiles from first to tile before last, until a tile is found
    while (temp->next != nullptr) {
        if (temp->next->contains(x,y) && !foundNode) {
            foundNode = true;
            nodeToMove = temp->next;
            // if last node needs to be raised
            if(temp->next->next == nullptr) {
                // update with new tail
                temp->next = nullptr;
                nodeToMove->next = front;
                front = nodeToMove;
                return true;
            } else {
                temp->next = nodeToMove->next;
                nodeToMove->next = front;
                front = nodeToMove;
                return true;
            }
        }
        // increment loop
        temp = temp->next;
    }
    // never found a tile
    return false;
}

/* Removes the topmost clicked tile from the list, when remove
 * button is selected.
 * Loops through list until topmost tile is found, then removes and returns it.
 * Runs in O(N).
 * Params -- x: top x coordinate of the tile
 *           y: left y coordinate of the tile
 * Return -- pointer to node that was removed if it exists, else
 *           returns nullptr */
TileNode* TileList::remove(int x, int y) {
    bool foundNode = false;
    TileNode* nodeToRemove = nullptr;
    TileNode* temp = front;

    // if front is null
    if(front == nullptr) {
        return front;
    }
    // if we need to remove front
    if(front->contains(x, y)) {
        front = front->next;
        return temp;
    }
    // check tiles from first to tile before last, until a tile is found
    while (temp->next != nullptr) {
        if (temp->next->contains(x,y) && !foundNode) {
            foundNode = true;
            nodeToRemove = temp->next;
            // if last node needs to be removed
            if(temp->next->next == nullptr) {
                // update with new tail
                temp->next = nullptr;
                return nodeToRemove;
            } else {
                temp->next = nodeToRemove->next;
                return nodeToRemove;
            }
        }
        // increment loop
        temp = temp->next;
    }
    // never found a tile
    return nullptr;

}

/* Removes all tiles that touch the given
 * x/y coordinates.
 * Runs in O(N).
 * Params -- x: top x coordinate of the tile
 *           y: left y coordinate of the tile
 * Return -- */
int TileList::removeAll(int x, int y) {
    int count = 0;
    TileNode* temp = front;
    TileNode* previous = front;
    // loop through list
    while(temp != nullptr) {
        if(temp->contains(x, y)) {
            // if it is the front
            if(temp == front) {
                front = temp->next;
                delete temp;
                temp = temp->next;
                count++;
            } else {
                previous->next = temp->next;
                delete temp;
                temp = previous->next;
                count++;
            }
        } else {
            // increment loop
            previous = temp;
            temp = temp->next;
        }
    }
    // return how many were removed
    return count;
}

/* Removes all tiles from list by looping through the list and deleting from memory.
 * Runs in O(N). */
void TileList::clear() {
    TileNode* temp = front;
    while (front != nullptr) {
        temp = front;
        front = front->next;
        delete temp;
    }
    delete front;
}
