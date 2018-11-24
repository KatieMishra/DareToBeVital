/*
 * CS 106B/X, Marty Stepp
 * This file declares the TreeNode structure type,
 * each object of which represents a single binary tree node
 * storing an integer and pointers to two other nodes.
 */

#ifndef _treenode_h
#define _treenode_h

/*
 * A structure to represent a single node of the tree.
 */
struct TreeNode {
    int data;
    TreeNode* left;
    TreeNode* right;

    /*
     * Construct a tree node with optional data, left, and right children.
     */
    TreeNode(int data = 0, TreeNode* left = nullptr, TreeNode* right = nullptr) {
        this->data = data;
        this->left = left;
        this->right = right;
    }
    
    /*
     * Returns true if this node is a leaf (no children).
     */
    bool isLeaf() const {
        return left == nullptr && right == nullptr;
    }
};

#endif // _treenode_h
