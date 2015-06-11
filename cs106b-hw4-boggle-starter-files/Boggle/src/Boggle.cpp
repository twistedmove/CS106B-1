#include "Boggle.h"
#include "shuffle.h"
#include "random.h"
#include "foreach.h"
#include "bogglegui.h"
#include "strlib.h"
#include <algorithm>
#include <cstring>

using namespace std;
using namespace BoggleGUI;

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

//initializes as many instance varilables as it can
Boggle::Boggle(Lexicon& dictionary, string boardText)
    : dict(dictionary)
{
    if (boardText.length() > 0) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                grid[i][j] = boardText[i * 4 + j];
    }
    else {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                grid[i][j] = CUBES[i * 4 + j][randomInteger(0, 5)];
        for (int i = 0; i < 100; i++) {
            int pos1 = randomInteger(0, 15),
                    pos2 = randomInteger(0, 15);
            //random shuffle, since i don't want to use shuffle(array,array_size)
            swap(grid[pos1 / 4][pos1 % 4], grid[pos2 / 4][pos2 % 4]);
        }
    }
    human_score = 0;
    computer_score = 0;
}

//lookup the letter at this location
char Boggle::getLetter(int row, int col) {
    return grid[row][col];
}

//check to see if the word should even be searched
bool Boggle::checkWord(string word) {
    if (word.length() < 4)
        return false;
    if (found_words.count(word) > 0)
        return false;
    if (!dict.contains(word))
        return false;
    return true;
}

//search if the word is searchable
bool Boggle::humanWordSearch(string word) {
    word = toUpperCase(word);
    if (!checkWord(word))
        return false;

    bool visited[4][4];
    //cycle through the board
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            memset(visited, 0, sizeof(visited));
            if (searchWord(i, j, word, 0, visited))
            {
                found_words.insert(word);
                human_score += word.length() - 3;
                return true;
            }
        }
    return false;
}

//helper function for humanWordSearch()
bool Boggle::searchWord(int row, int col, string word, int current, bool visited[4][4]) {
    if (visited[row][col])
        return false;
    if (word.length() == current)
        return true;
    if (grid[row][col] != word[current])
        return false;

    visited[row][col] = true;

    //double for loop
    bool found = false;
    for (int r = 0; !found && r < 8; r++) {
        int oi = row + di[r], oj = col + dj[r];
        if (on_board(oi, oj) &&
            (searchWord(oi, oj, word, current + 1, visited)))
        {
            labelCube(row, col, grid[row][col], true);
            found = true;
        }
    }

    visited[row][col] = false;

    return found;
}

//helper function for computerWordSearch()
void Boggle::searchWord2(int row, int col, string word, bool visited[4][4], Set<string>& result) {
    if (visited[row][col])
        return;
    word += getLetter(row, col);
    if (!dict.containsPrefix(word))
        return;
    if (checkWord(word))
        result.add(word);

    visited[row][col] = true;

    //cycle through every letter
    for (int r = 0; r < 8; r++) {
        int oi = row + di[r], oj = col + dj[r];
        if (on_board(oi, oj))
            searchWord2(oi, oj, word, visited, result);
    }

    visited[row][col] = false;
}

bool Boggle::on_board(int i, int j) {
    return i >= 0 && i < 4 && j >=0 && j < 4;
}

int Boggle::humanScore() {
    return human_score;
}

//generate and search all words
Set<string> Boggle::computerWordSearch() {
    Set<string> result;
    bool visited[4][4] = {0};
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            memset(visited, 0, sizeof(visited));//use a grid instead
            searchWord2(i, j, "", visited, result);
        }
    for (string s: result)
        computer_score += s.length() - 3;
    return result;
}

int Boggle::getScoreComputer() {
    return computer_score;
}

ostream& operator<<(ostream& out, Boggle& boggle) {
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            out << boggle.getLetter(i, j);
        out << endl;
    }
    return out;
}
