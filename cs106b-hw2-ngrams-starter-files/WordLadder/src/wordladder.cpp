#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "set.h"
#include "simpio.h"
#include "queue.h"
#include "stack.h"

using namespace std;

int main() {
    setConsoleSize(750, 450);
    setConsoleFont("Monospaced-Bold-16");
    setConsoleEcho(true);
    setConsolePrintExceptions(true);

    //read and store dictionary file
    ifstream fin("dictionary.txt");
    string word;
    Set<string> dict;
    while (fin >> word)
        dict.add(toLowerCase(word));

    cout << "Welcome to CS 106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time." << endl << endl;

    bool quit = false;
    while (!quit)
    {
        string word1;
        while (true)
        {
            word1 = getLine("Word #1 (or Enter to quit): ");
            //exit the program
            if (word1.length() == 0)
            {
                quit = true;
                break;
            }
            //check whether the input is a word
            word1 = toLowerCase(word1);
            if (!dict.contains(word1))
                cout << word1 << " is not a word" << endl;
            else
                break;
        }
        if (quit) break;

        string word2;
        while (true)
        {
            word2 = getLine("Word #2 (or Enter to quit): ");
            //exit the program
            if (word2.length() == 0)
            {
                quit = true;
                break;
            }
            //check whether the input is a word
            word2 = toLowerCase(word2);
            if (!dict.contains(word2))
                cout << word2 << " is not a word" << endl;
            else
                break;
        }
        if (quit) break;

        //invalid input: words should have the same length
        if (word1.length() != word2.length())
        {
            cout << "The two words must be the same length." << endl;
            continue;
        }

        //store partial letters that represent possibilities to explore
        Queue<Stack<string>> q;
        Stack<string> s, ans;
        Set<string> visited;
        bool done = false;
        s.push(word1);
        q.enqueue(s);
        visited.add(word1);
        while (!done && !q.isEmpty())
        {
            Stack<string> curr = q.dequeue();
            string p = curr.peek();

            //loop which goes through each character index in the word
            for (size_t i = 0; !done && i < p.length(); i++)
            //loop which goes through the letter alphabet a-z, replacing the character in the index position with each letter in turn
                for (int c = 'a'; !done && c <= 'z'; c++)
                {
                    if (p[i] != c)
                    {
                        string neighbor = p;
                        neighbor[i] = c;
                        if (dict.contains(neighbor) && !visited.contains(neighbor))
                        {
                            visited.add(neighbor);
                            curr.push(neighbor);
                            if (neighbor == word2)
                            {
                                done = true;
                                ans = curr;
                            }
                            else
                            {
                                q.enqueue(curr);
                                curr.pop();
                            }
                        }
                    }
                }
        }
        if (done)
        {
            cout << "A ladder from " << word2 << " back to " << word1 << ":" << endl;
            while (!ans.isEmpty())
                cout << ans.pop() << " ";
            cout << endl;
        }
        //invalid input: two words should not be the same
        else
            cout << "The two words must be different." << endl;

        cout << endl;
    }

    cout << "Have a nice day." << endl;
    return 0;
}
