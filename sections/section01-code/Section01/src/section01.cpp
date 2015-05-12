/*
 * CS 106B Section 01 pre-problems
 */

#include <cmath>
#include <iostream>
#include <string>
#include "console.h"
#include "grid.h"
#include "vector.h"
using namespace std;

// Here are the functions for you to implement.
// Write the function bodies below.
double mean(Vector<double>& data);                     // Exercise 5.2
void reshape(Grid<int>& grid, int nRows, int nCols);   // Exercise 5.11


int main() {
    setConsoleSize(750, 500);
    setConsoleFont("Monospaced-Bold-14");
    setConsoleEcho(true);

    // here are some example calls to test your code
    Vector<double> v;
    v += 1.0, 3.25, 2.5, 8.25, 15.0, -2.0, 35.5, 44.5;
    cout << "mean: " << mean(v) << endl;               // should be 13.5
    v += 0.0, 2.0;
    cout << "mean: " << mean(v) << endl;               // should be 11.0
    cout << endl;

    Grid<int> grid;      // create a 3x4 grid storing numbers 1-12 as in book
    grid.resize(3, 4);
    int num = 1;
    for (int r = 0; r < grid.numRows(); r++) {
        for (int c = 0; c < grid.numCols(); c++) {
            grid[r][c] = num;
            num++;
        }
    }
    cout << grid << endl;
    reshape(grid, 4, 3);
    cout << grid << endl;
    reshape(grid, 2, 5);
    cout << grid << endl;
    reshape(grid, 1, 1);
    cout << grid << endl;

    cout << "Have a nice Life!" << endl;
    return 0;
}

double mean(Vector<double>& data) {
    double sum = 0;
    for (int i = 0; i < data.size(); i++) {
        sum += data[i];
    }
    double average = sum / data.size();
    return average;
}

void reshape(Grid<int>& grid, int nRows, int nCols) {
    Grid<int> newGrid(nRows, nCols);
    int sourceRow = 0, sourceCol = 0;
    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {
            newGrid[i][j] = grid[sourceRow][sourceCol];
            sourceCol++;
            // If we've filled up this row we need to move on to the next one
            if (sourceCol == grid.nCols) {
                // If we've filled up all rows we're done
                if (sourceRow == grid.nRows) {
                    grid = newGrid;
                    return;
                }
                sourceCol = 0;
                sourceRow++;
            }
        }
    }
    grid = newGrid;
}
