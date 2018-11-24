/*
 * CS 106B/X, Marty Stepp
 *
 * This file is a client that uses our binary tree.
 */

#include <iostream>
#include "console.h"
#include "treenode.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// BEGIN TREE FUNCTIONS
///////////////////////////////////////////////////////////////////////////////

/*
 * Prints all nodes of the tree with the given node as its root.
 */
void print(TreeNode* root) {
    if (root != nullptr) {
        cout << root->data << endl;     // pre-order: current before children
        print(root->left);
        print(root->right);
    }
}

/*
 * Returns the number of nodes in the given tree.
 */
int size(TreeNode* node) {
    if (node == nullptr) {
        return 0;
    } else {
        return size(node->left) + size(node->right) + 1;
    }
}

/*
 * Returns true if the given tree contains the given value.
 */
bool contains(TreeNode* node, int value) {
    if (node == nullptr) {
        return false;
    } else if (node->data == value) {
        return true;
    } else {
        return contains(node->left, value)
            || contains(node->right, value);
    }
}

/*
 * Returns the given tree's height.
 * The height of an empty tree is defined to be 0.
 * An unempty tree's height is the length of the longest path from the root
 * node to any other node in the tree.
 */
int height(TreeNode* node) {
    // TODO
    return 0;
}

/*
 * Prints the elements of the given tree horizontally and indented.
 */
void printSideways(TreeNode* node) {
    // TODO
}

///////////////////////////////////////////////////////////////////////////////
// END TREE FUNCTIONS
///////////////////////////////////////////////////////////////////////////////

int main() {
    /*         9
     *       /   \
     *     6       14
     *    / \     / \
     *  -3   7  11   19
     *           \
     *            88
     */
    // build a tree by connecting nodes
    TreeNode* root = new TreeNode(9);
    root->left = new TreeNode(6);
    root->right = new TreeNode(14);
    root->left->left = new TreeNode(-3);
    root->left->right = new TreeNode(7);
    root->right->left = new TreeNode(11);
    root->right->right = new TreeNode(19);
    root->right->left->right = new TreeNode(88);

    // print all the values in the tree
    print(root);


    // TODO
    
    // test print function
    cout << "========= print =========" << endl;
    print(root);
    cout << endl;

    cout << "========= print =========" << endl;
    TreeNode* root2 = nullptr;
    print(root2);
    cout << endl;

    cout << "========= print =========" << endl;
    TreeNode* root3 = new TreeNode(42);
    print(root3);
    cout << endl;

    // test size function
    cout << "========= size =========" << endl;
    cout << size(root) << endl;

    // test contains function
    cout << "======= contains ======== " << endl;
    cout << "Contains  9: " << boolalpha << contains(root,  9) << endl;   // true
    cout << "Contains  7: " << boolalpha << contains(root,  7) << endl;   // true
    cout << "Contains 14: " << boolalpha << contains(root, 14) << endl;   // true
    cout << "Contains 19: " << boolalpha << contains(root, 19) << endl;   // true
    cout << "Contains 87: " << boolalpha << contains(root, 87) << endl;   // false
    cout << "Contains  0: " << boolalpha << contains(root,  0) << endl;   // false
    cout << "Contains 60: " << boolalpha << contains(root, 60) << endl;   // false
    cout << "Contains -1: " << boolalpha << contains(root, -1) << endl;   // false

    // test printSideways function
    cout << "===== printSideways =====" << endl;
    printSideways(root);

    return 0;
}
