/*
 * File: shuffle.h
 * ---------------
 * This file contains implementation of a shuffling function that operates on
 * a 1-D and 2-D array, Vector, or Grid of any type.
 *
 * Author : Marty Stepp
 */

#ifndef _shuffle_h
#define _shuffle_h

#include "grid.h"
#include "random.h"
#include "vector.h"

/*
 * Randomly rearranges the elements of the given array up to the given length.
 * Precondition: The array pointer points to a valid non-NULL array in memory,
 * and that array contains at least 'length' elements.
 */
template <typename T>
void shuffle(T* array, int length) {
    for (int i = 0; i < length; i++) {
        int j = randomInteger(i, length - 1);
        if (i != j) {
            T temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }
    }
}

/*
 * Randomly rearranges the elements of the given 2-D array up to the given
 * number of rows and columns.
 * Precondition: The array pointer points to a valid non-NULL 2-D array in
 * memory, and that array contains at least the given number of rows/columns.
 */
template <typename T>
void shuffle(T** array2d, int rows, int cols) {
    int length = rows * cols;
    for (int i = 0; i < length; i++) {
        int j = randomInteger(i, length - 1);
        if (i != j) {
            int r1 = i / cols;
            int c1 = i % cols;
            int r2 = j / cols;
            int c2 = j % cols;

            T temp = array2d[r1][c1];
            array2d[r1][c1] = array2d[r2][c2];
            array2d[r2][c2] = temp;
        }
    }
}

/*
 * Randomly rearranges the characters of the given string and returns the
 * rearranged version.
 */
std::string shuffle(std::string s) {
    for (int i = 0, length = s.length(); i < length; i++) {
        int j = randomInteger(i, length - 1);
        if (i != j) {
            std::string::value_type temp = s[i];
            s[i] = s[j];
            s[j] = temp;
        }
    }
    return s;
}

/*
 * Randomly rearranges the elements of the given vector.
 */
template <typename T>
void shuffle(Vector<T>& v) {
    for (int i = 0, length = v.size(); i < length; i++) {
        int j = randomInteger(i, length - 1);
        if (i != j) {
            T temp = v[i];
            v[i] = v[j];
            v[j] = temp;
        }
    }
}

/*
 * Randomly rearranges the elements of the given grid.
 */
template <typename T>
void shuffle(Grid<T>& grid) {
    int rows = grid.numRows();
    int cols = grid.numCols();
    int length = rows * cols;
    for (int i = 0; i < length; i++) {
        int j = randomInteger(i, length - 1);
        if (i != j) {
            int r1 = i / cols;
            int c1 = i % cols;
            int r2 = j / cols;
            int c2 = j % cols;

            T temp = grid[r1][c1];
            grid[r1][c1] = grid[r2][c2];
            grid[r2][c2] = temp;
        }
    }
}

#endif
