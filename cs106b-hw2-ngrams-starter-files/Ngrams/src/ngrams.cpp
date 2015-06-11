#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "simpio.h"
#include "random.h"
#include "map.h"
#include "vector.h"

using namespace std;
bool operator < (const Vector<string>& v1, const Vector<string>& v2);

bool operator < (const Vector<string>& v1, const Vector<string>& v2)
{
    for (int i = 0; i < min(v1.size(), v2.size()); i++)
    {
        if (v1.get(i) < v2.get(i))
            return true;
        else if (v1.get(i) > v2.get(i))
            return false;
    }
    return v1.size() < v2.size();
}

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);

    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl << endl;

    //read file
    string file;
    ifstream fin(file.c_str());
    while (true) {
        file = getLine("Input file name? ");
        if (openFile(fin, file))
            break;
        cout << "File not found; please try again." << endl;
    }
    int N;
    while (true) {
        N = getInteger("Value of N? ");
        if (N < 2)
            cout << "N must be bigger or equal to 2." << endl;
        else
            break;
    }

    cout << endl;
    Vector<string> words;
    string word;
    while (fin >> word)
        words.add(word);

    //Keep a window of N-1 words
    Vector<string> window;
    for (int i = 0; i < N - 1; i++)
        window.add(words[i]);

    Vector<Vector<string> > keys;
    Map<Vector<string>, Vector<string> > map;
    for (int i = 0, j = N - 1; i < words.size(); i++, j++)
    {
        Vector<string> v;
        if (map.containsKey(window))
            v = map.get(window);
        else
            keys.add(window);

        v.add(words[j % words.size()]);
        map.put(window, v);

        //discard first word from the window, append the new word
        window.add(words[j % words.size()]);
        window.remove(0);
    }

    while (true)
    {
        int len = getInteger("# of random words to generate (0 to quit)? ");
        if (len == 0)
            break;
        if (len < N) {
            cout << "Must be at least " << N << " words." << endl;
        }
        else {
            window = keys[randomInteger(0, keys.size() - 1)];
            cout << "... ";
            for (int i = 0; i < window.size(); i++)
                cout << window[i] << " ";
            for (len = len - (N - 1); len > 0; len--)
            {
                //look up all possible words
                Vector<string> candidates = map.get(window);
                string next = candidates[randomInteger(0, candidates.size() - 1)];
                cout << next << " ";
                //discarding first word in the window, appending the new suffix
                window.add(next);
                window.remove(0);
            }
            cout << "..." << endl << endl;
        }
    }

    cout << "Exiting." << endl;
    return 0;
}
