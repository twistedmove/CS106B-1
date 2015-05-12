/*
 * CS 106B Pre-section problems
 * This file contains the declaration of the TreeNode structure.
 * A TreeNode stores one integer piece of data in a binary tree of integers.
 */

#ifndef _treenode_h
#define _treenode_h

struct TreeNode {
    int data;
    TreeNode* left;
    TreeNode* right;

    /*
     * Constructs a new tree node with the given data and left/right links.
     */
    TreeNode(int data, TreeNode* left = NULL, TreeNode* right = NULL) {
        this->data = data;
        this->left = left;
        this->right = right;
    }
    
    /*
     * Returns true if this node has no children.
     */
    bool isLeaf() {
        return left == NULL && right == NULL;
    }
};

#endif
