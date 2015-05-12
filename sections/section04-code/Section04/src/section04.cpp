/*
 * CS 106B Section 4 pre-problem
 *
 * Expected output, once you have it working:
 * diceSum(3, 7):
 * {1, 1, 5}
 * {1, 2, 4}
 * {1, 3, 3}
 * {2, 2, 3}
 *
 * diceSum(4, 13):
 * {1, 1, 5, 6}
 * {1, 2, 4, 6}
 * {1, 2, 5, 5}
 * {1, 3, 3, 6}
 * {1, 3, 4, 5}
 * {1, 4, 4, 4}
 * {2, 2, 3, 6}
 * {2, 2, 4, 5}
 * {2, 3, 3, 5}
 * {2, 3, 4, 4}
 * {3, 3, 3, 4}
 */

#include <cmath>
#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
#include "stack.h"
#include "strlib.h"
using namespace std;

// You must modify the diceSumHelper as described on the web site.
// You may change the function parameters and code as necessary.
void diceSum(int dice, int desiredSum);
void diceSumHelper(int dice, int desiredSum, int sumSoFar, Stack<int>& chosen);

// here are some example calls to test your code
int main() {
    cout << "diceSum(3, 7):" << endl;
    diceSum(3, 7);
    cout << endl;
    cout << "diceSum(4, 13):" << endl;
    diceSum(4, 13);
    return 0;
}

/*
 * Prints all possible outcomes of rolling the given
 * number of six-sided dice that add up to the given
 * integer sum, in {#, #, #} format.
 */
void diceSum(int dice, int desiredSum) {
    Stack<int> chosen;
    diceSumHelper(dice, desiredSum, 0, 0, chosen);
}

/*
 * Recursive helper to implement diceSum behavior.
 * TODO: Modify me so that I don't print duplicate sums.
 */
void diceSumHelper(int dice, int desiredSum, int sumSoFar, int largestSoFar, Stack<int>& chosen) {
    if (dice == 0) {
        // base case: no dice left to roll
        if (sumSoFar == desiredSum) {
            cout << chosen << endl;
        }
    } else {
        // for each of the possible values I could choose for that die,
        for (int i = max(1, largestSoFar); i <= 6; i++) {
            // choose
            chosen.push(i);

            // explore
            int minPossibleSum = sumSoFar + i + (dice-1)*1;
            int maxPossibleSum = sumSoFar + i + (dice-1)*6;
            if (desiredSum >= minPossibleSum &&
                desiredSum <= maxPossibleSum) {
                int newSoFar = sumSoFar + i;
                int newLargest = max(largestSoFar, i);
                diceSumHelper(dice - 1, desiredSum, newSoFar, newLargest, chosen);
            }

            // un-choose (backtrack)
            chosen.pop();
        }
    }
}
