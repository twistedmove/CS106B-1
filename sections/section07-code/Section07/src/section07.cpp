/*
 * CS 106B Pre-section problems
 * You can write your solution in this file below.
 * You may also need to modify BinaryTree.h to declare function prototypes
 * depending on your implementation.
 * See the bottom of this file for expected output, or look at the tree
 * diagrams in the comments to see what the tree structure should be.
*/

#include <cmath>
#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
#include "stack.h"
#include "strlib.h"
#include "TreeNode.h"
#include "BinaryTree.h"
using namespace std;

// here are some example calls to test your code
int main() {
    // create an initial tree state
    TreeNode* root = new TreeNode(7);
    root->left = new TreeNode(3);
    root->right = new TreeNode(9);
    root->left->left = new TreeNode(1);
    root->left->right = new TreeNode(4);
    root->right->left = new TreeNode(6);
    root->right->right = new TreeNode(8);
    root->right->right->right = new TreeNode(0);
    BinaryTree tree(root);

    cout << "initial tree:" << endl;
    tree.printSideways();
                                                         //        7
    tree.removeLeaves();                                 //      /   \
    cout << "after call #1 to removeLeaves:" << endl;    //    3       9
    tree.printSideways();                                //             \
    cout << endl;                                        //              8

    tree.removeLeaves();
    cout << "after call #2 to removeLeaves:" << endl;    //        7
    tree.printSideways();                                //          \
    cout << endl;                                        //            9

    tree.removeLeaves();
    cout << "after call #3 to removeLeaves:" << endl;    //        7
    tree.printSideways();
    cout << endl;

    tree.removeLeaves();
    cout << "after call #4 to removeLeaves:" << endl;    //        null
    tree.printSideways();

    return 0;
}

bool BinaryTree::removeLeaves() {
    removeLeavesHelper(root);
}

bool BinaryTree::removeLeavesHelper(TreeNode* &node){
    if (node != NULL){
    if (node->isLeaf()){
        delete node;
        node = NULL;
    } else{
        removeLeavesHelper(node->left);
        removeLeavesHelper(node->right);
}
}
}







/*
Expected output, once you have it working:

initial tree
            0
        8
    9
        6
7
        4
    3
        1
after call #1 to removeLeaves:
        8
    9
7
    3

after call #2 to removeLeaves:
    9
7

after call #3 to removeLeaves:
7

after call #4 to removeLeaves:

*/
