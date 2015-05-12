/*
 * CS 106B Section 3 pre-problems (solution)
 */

#include <cmath>
#include <iostream>
#include <string>
#include "console.h"
#include "gwindow.h"
#include "simpio.h"
#include "strlib.h"
using namespace std;

// Here are the functions for you to implement.
// Write the function bodies below.

int digitSum(int n);   // Exercise 7.7
void drawRuler(GWindow& window, int x, int y, int width, int height, int levels);      // Exercise 8.14


int main() {
    setConsoleSize(750, 500);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);

    // here are some example calls to test your code
    cout << "digitSum(8)     = " << digitSum(8)     << endl;   // 8
    cout << "digitSum(12345) = " << digitSum(12345) << endl;   // 15
    cout << "digitSum(8517)  = " << digitSum(8517)  << endl;   // 21
    cout << "digitSum(-123)  = " << digitSum(-123)  << endl;   // -6
    cout << "digitSum(0)     = " << digitSum(0)     << endl;   // 0
    cout << endl;

    GWindow window(420, 350);
    drawRuler(window, 20,  50, 200, 80, 4);
    drawRuler(window, 80, 250, 300, 64, 6);

    return 0;
}

int digitSum(int n) {

    if (n < 0)
            return -digitSum(-n);
        if (n == 0)
            return 0;
        else
        return digitSum(n / 10) + n % 10;

}


void drawRuler(GWindow& window, int x, int y, int width, int height, int levels) {

    if (levels >=0)
    {
        window.drawLine(x, y + height, x + width, y + height);
        window.drawLine(x + width/2, y, x + width/2, y + height);
        drawRuler(window, x, y + height/2, width/2, height/2, levels - 1);
        drawRuler(window, x + width/2, y + height/2, width/2, height/2, levels - 1);
    }
}
