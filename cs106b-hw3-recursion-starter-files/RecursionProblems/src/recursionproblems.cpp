#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "gwindow.h"
#include "hashmap.h"
#include "map.h"
#include "random.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
#include "recursionproblems.h"
using namespace std;

int countKarelPaths(int street, int avenue) {
    return 0;
}

int convertStringToInteger(string exp) {
    if (exp.length() == 0)
        return 0;
    if (exp[0] == '-')
        return -convertStringToInteger(exp.substr(1));
    else
        return convertStringToInteger(exp.substr(0, exp.length() - 1)) * 10 + (exp[exp.length() - 1] - '0');
}

bool isBalanced(string exp) {
    if (exp.length() == 0)
        return true;

    int i, original_length = exp.length();
    i = exp.find("()");
    if (i != exp.npos)
        exp.erase(i, 2);
    i = exp.find("{}");
    if (i != exp.npos)
        exp.erase(i, 2);
    i = exp.find("[]");
    if (i != exp.npos)
        exp.erase(i, 2);

    if (exp.length() == original_length)
        return false;

    return isBalanced(exp);
}

void drawSierpinskiTriangle(GWindow& gw, int x, int y, int size, int order) {
    if (order == 0)
        return;

    double coff = sqrt(3) / 2;
    gw.drawLine(x, y, x + size, y);
    gw.drawLine(x, y, x + (double) size / 2, y + coff * size);
    gw.drawLine(x + size, y, x + (double) size / 2, y + coff * size);

    drawSierpinskiTriangle(gw, x, y, size / 2, order - 1);
    drawSierpinskiTriangle(gw, x + (double) size / 2, y, size / 2, order - 1);
    drawSierpinskiTriangle(gw, x + (double) size / 4, y + coff / 2 * size, size / 2, order - 1);
}

void floodFill(int x, int y, int width, int height, int color) {
    int original_color = getPixelColor(x, y);
    setPixelColor(x, y, color);
    if (x + 1 < width && getPixelColor(x + 1, y) == original_color)
        floodFill(x + 1, y, width, height, color);
    if (x - 1 >= 0 && getPixelColor(x - 1, y) == original_color)
        floodFill(x - 1, y, width, height, color);
    if (y + 1 < height && getPixelColor(x, y + 1) == original_color)
        floodFill(x, y + 1, width, height, color);
    if (y - 1 >= 0 && getPixelColor(x, y - 1) == original_color)
        floodFill(x, y - 1, width, height, color);
}

int stringToId(string s, vector<vector<vector<int> > >& bnf, map<string, int>& f_map, vector<string>& rev_map)
{
    if (f_map.count(s) > 0)
        return f_map.find(s)->second;

    int id = bnf.size();
    f_map.insert(make_pair(s, id));
    rev_map.push_back(s);
    bnf.push_back(vector<vector<int> >());

    return id;
}

void grammar_helper(vector<vector<vector<int> > >& bnf, int start, vector<int>& result)
{
    if (bnf[start].size() == 0)
    {
        result.push_back(start);
        return;
    }

    int j = randomInteger(0, bnf[start].size() - 1);
    for (int i = 0; i < bnf[start][j].size(); i++)
        grammar_helper(bnf, bnf[start][j][i], result);
}

Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    if (symbol == "")
        throw string("symbol is illegal");

    vector<vector<vector<int> > > bnf;
    map<string, int> f_map;
    vector<string> rev_map;

    string line;
    while (getline(input, line))
    {
        int delim = line.find("::=");
        string non_term = line.substr(0, delim);
        if (f_map.count(non_term) == 1 && bnf[f_map.find(non_term)->second].size() > 0)
            throw string("duplicate non-terminal found");
        delim += 3;
        while (true)
        {
            int j = bnf[stringToId(non_term, bnf, f_map, rev_map)].size();
            bnf[stringToId(non_term, bnf, f_map, rev_map)].push_back(vector<int>());

            int next_delim = line.find("|", delim);
            string production = line.substr(delim, next_delim == string::npos ? string::npos : next_delim - delim);
            int last_space = 0;
            while (true)
            {
                int next_space = production.find(" ", last_space);
                string s = production.substr(last_space, next_space == string::npos ? string::npos : next_space - last_space);
                int id1 = stringToId(non_term, bnf, f_map, rev_map);
                int id2 = stringToId(s, bnf, f_map, rev_map);
                bnf[id1][j].push_back(id2);
                if (next_space == string::npos)
                    break;
                last_space = next_space + 1;
            }
            if (next_delim == string::npos)
                break;
            delim = next_delim + 1;
        }
    }

    if (f_map.count(symbol) == 0)
        throw string("symbol is not a non-teminator or a terminator");

    Vector<string> v;
    for (int j = 0; j < times; j++)
    {
        vector<int> result;
        grammar_helper(bnf, stringToId(symbol, bnf, f_map, rev_map), result);
        string s;
        for (int i = 0; i < result.size(); i++)
            s += rev_map[result[i]] + " ";
        v.push_back(s);
    }
    return v;
}
