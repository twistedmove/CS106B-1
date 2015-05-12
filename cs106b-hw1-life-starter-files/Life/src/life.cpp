#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "lifegui.h"
using namespace std;

void storeIntoGrid(string filename, Grid<string>& grid);
void printGrid(Grid<string> grid);
void oneIteration(Grid<string>& grid);
void calculateNeighbour(int i, int j, Grid<string> grid, Grid<int>& virusCount);
void updateGrid(int i, int j, Grid<string>& grid, Grid<int> virusCount);

int main() {
    setConsoleSize(750, 500);
    setConsoleFont("Monospaced-Bold-14");
    setConsoleEcho(true);

    cout << "Welcome to the CS 106B Game of Life," << endl;
    cout << "a simulation of the lifecycle of a bacteria colony" << endl;
    cout << "Cells (X) live and die by the following rules:" << endl;
    cout << "- A cell with 1 or fewer neighbors dies." << endl;
    cout << "- Locations with 2 neighbors remain stable." << endl;
    cout << "- Locations with 3 neighbors will create life." << endl;
    cout << "- A cell with 4 or more neighbors dies." << endl << endl;

    ifstream input;
    string filename;
    bool check = false;
    do {
        filename = getLine("Grid input file name? ");
        check = openFile(input, filename);
        if (!check) {
            cout << "File not found; please try again." << endl;
        }
    }
    while (!check);

    Grid<string> grid;
    storeIntoGrid(filename, grid);

    while(true) {
        printGrid(grid);
        string input =  getLine("a)nimate, t)ick, q)uit? ");
        if (input == "q") {
            cout << "Have a nice Life!" << endl;
            break;
        }
        else if (input == "t") {
            oneIteration(grid);
        }
        else if (input == "a") {
            int iteration = getInteger("How many frames? ");
            for (int i = 0; i < iteration; i++) {
                oneIteration(grid);
                clearConsole();
                printGrid(grid);
                pause(50);
            }
        }
        else {
            cout << "Please enter a, t or q" << endl;
        }
    }

    return 0;
}

// store file into the starting grid
void storeIntoGrid(string filename, Grid<string>& grid) {
    ifstream fin;
    string row, column;
    openFile(fin, filename);
    getline(fin, row);
    getline(fin, column);
    int nrow = stringToInteger(row);
    int ncol = stringToInteger(column);
    grid.resize(nrow, ncol);
    string line;
    for (int i = 0; i < nrow; i++) {
        getline(fin, line);
        for (int j = 0; j < ncol; j++) {
            grid[i][j] = line[j];
        }
    }
}

// print grid after each iteration
void printGrid(Grid<string> grid) {
    for (int i = 0; i < grid.numRows(); i++) {
        string line = "";
        for (int j = 0; j < grid.numCols(); j++) {
            line += grid[i][j];
        }
        cout << line << endl;
    }
}

// calculate neighbours first, then update grid
void oneIteration(Grid<string>& grid) {
    Grid<int> virusCount(grid.numRows(), grid.numCols());
    for (int i = 0; i < grid.numRows(); i++) {
        for (int j = 0; j < grid.numCols(); j++) {
            calculateNeighbour(i, j, grid, virusCount);
        }
    }
    for (int i = 0; i < grid.numRows(); i++) {
        for (int j = 0; j < grid.numCols(); j++) {
            updateGrid(i, j, grid, virusCount);
        }
    }
}

// store calculated number into virusCount
void calculateNeighbour(int i, int j, Grid<string> grid, Grid<int>& virusCount) {
    int count = 0;
    for (int x = i - 1; x < i + 2; x++) {
        for (int y = j - 1; y < j + 2; y++) {
            if (grid.inBounds(x, y) && (x != i || y != j)) {
                if (grid[x][y] == "X") {
                    count++;
                }
            }
        }
    }
    virusCount[i][j] = count;
}

// update grid according to value in virusCount
void updateGrid(int i, int j, Grid<string>& grid, Grid<int> virusCount) {
    if (virusCount[i][j] == 0 || virusCount[i][j] == 1 || virusCount[i][j] >= 4) {
        grid[i][j] = "-";
    }
    else if (virusCount[i][j] == 3) {
        grid[i][j] = "X";
    }
}

