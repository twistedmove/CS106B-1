/*
 * CS 106B Pre-section problems
 * This file contains the implementation of members of the BinaryTree class,
 * which defines a binary tree of integers.
 * See BinaryTree.h for a description of each member.
 */

#include "BinaryTree.h"
#include "random.h"
#include "strlib.h"

BinaryTree::BinaryTree(TreeNode* initialRoot) {
    root = initialRoot;
}

bool BinaryTree::contains(int value) {
    return containsHelper(root, value);
}

bool BinaryTree::containsHelper(TreeNode* node, int value) {
    if (node == NULL) {
        return false;
    } else {
        // non-null node; might have children
        return (node->data == value)
                || containsHelper(node->left, value)
                || containsHelper(node->right, value);
    }
}

void BinaryTree::print() {
    printHelper(root);
}

void BinaryTree::printHelper(TreeNode* node) {
    if (node != NULL) {
        // recursive case; non-null node; print it
        printHelper(node->left);
        cout << node->data << endl;
        printHelper(node->right);
    } // else base case, null, do nothing
}

void BinaryTree::printSideways() {
    printSidewaysHelper(root, "");
}

void BinaryTree::printSidewaysHelper(TreeNode* node, string indent) {
    if (node != NULL) {
        // recursive case; non-null node; print it
        printSidewaysHelper(node->right, indent + "    ");
        cout << indent << node->data << endl;
        printSidewaysHelper(node->left, indent + "    ");
    } // else base case, null, do nothing
}
