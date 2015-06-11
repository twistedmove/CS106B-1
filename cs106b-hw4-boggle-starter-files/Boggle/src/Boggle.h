#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include <set>
#include "lexicon.h"
using namespace std;

class Boggle {
public:
    Boggle(Lexicon& dictionary, string boardText = "");
    char getLetter(int row, int col);
    bool checkWord(string word);
    bool humanWordSearch(string word);
    int humanScore();
    Set<string> computerWordSearch();
    int getScoreComputer(); //use the same library

private:
    Lexicon& dict;
    char grid[4][4];
    set<string> found_words;
    int human_score, computer_score;

    //8 directions of possible movements, e.g. west, northwest, north...etc.
    int di[8] = {-1, -1, 0, 1, 1, 1, 0, -1}, dj[8] = {0, 1, 1, 1, 0, -1, -1, -1}; //use double loop 3-by-3

    bool searchWord(int row, int col, string word, int current, bool visited[4][4]);
    void searchWord2(int row, int col, string word, bool visited[4][4], Set<string>& result);
    bool on_board(int i, int j);
};

ostream& operator << (ostream& out, Boggle& boggle);

#endif
