/*
 * CS 106B Pre-section problems
 * This file contains the declaration of the BinaryTree class,
 * which defines a binary tree of integers.
 * See BinaryTree.cpp for implementation of each member.
 */

#ifndef _binarytree_h
#define _binarytree_h

#include <iostream>
#include <string>
#include "TreeNode.h"

using namespace std;

class BinaryTree {
public:
    /*
     * Constructs a new binary tree with the given root node pointer.
     */
    BinaryTree(TreeNode* initialRoot);
    
    /*
     * Returns true if the given value is contained in this tree.
     */
    bool contains(int value);
    
    /*
     * Outputs the elements of this binary tree, one per line.
     * This is an in-order traversal, visiting the left, then current, then right.
     */
    void print();
    
    /*
     * Prints the elements in a sideways fashion with indentation at each level
     * of the tree.  This is a reverse in-order traversal: right subtrees are
     * printed first, then parent nodes, then left subtrees.
     */
    void printSideways();

    /*
     * Your function to implement
     */
    bool removeLeaves();
    bool removeLeavesHelper(TreeNode* &node);


private:
    TreeNode* root;   // pointer to top overall node of tree; NULL if empty
    
    /*
     * Recursive helpers to implement the above public members.
     * Each helper accepts a pointer to the part of the tree to process.
     */
    bool containsHelper(TreeNode* node, int value);
    void printHelper(TreeNode* node);
    void printSidewaysHelper(TreeNode* node, string indent);
};

#endif
