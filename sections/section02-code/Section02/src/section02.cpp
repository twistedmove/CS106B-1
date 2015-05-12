/*
 * CS 106B Section 02 pre-problems
 */

#include <cmath>
#include <iostream>
#include <string>
#include "console.h"
#include "map.h"
#include "simpio.h"
#include "strlib.h"
using namespace std;

// Here are the functions for you to implement.
// Write the function bodies below.

// Exercise 5.19
void readMorseCodeMap(ifstream& input, Map<char, string>& codeMap);
void toMorseCode(Map<char, string>& codeMap, string text);


int main() {
    setConsoleSize(750, 500);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);

    // here is an example call to test your code
    ifstream input;
    input.open("morsecode.txt");
    Map<char, string> codeMap;
    readMorseCodeMap(input, codeMap);
    cout << "code map is: " << codeMap << endl << endl;

    cout << "SOS TITANIC:" << endl;
    toMorseCode(codeMap, "SOS TITANIC");
    cout << endl;
    cout << "WE ARE SINKING FAST:" << endl;
    toMorseCode(codeMap, "WE ARE SINKING FAST");

    return 0;
}

/*
 * Reads the given input file and stores it into a map from {char -> string}
 * representing conversion codes from normal text characters to Morse code.
 * For example, the character 'J' maps to the string ".---"
 */
void readMorseCodeMap(ifstream& input, Map<char, string>& codeMap) {
    string line;
    while (getline(input, line)) {
        codeMap[line[0]] = trim(line.substr(1));
    }
}

void toMorseCode(Map<char, string>& codeMap, string text) {
    for (int i = 0; i < (int) text.length(); i++) {
        if (codeMap.containsKey(text[i])) {
            cout << codeMap[text[i]];
        }
        cout << " ";
    }
    cout << endl;
}


