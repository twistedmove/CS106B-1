#include <iostream>
#include "Boggle.h"
#include "simpio.h"
#include "console.h"
#include "foreach.h"
#include "bogglegui.h"
#include "strlib.h"

using namespace BoggleGUI;
// sets the GUI window back to its initial state
void resetGUI(Boggle &boggle) {
    reset();
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            labelCube(i, j, boggle.getLetter(i, j), false);
}

// ask about user preference
void playOneGame(Lexicon& dictionary) {
    string ans = getLine("Do you want to generate a random board? ");
    string initial;


    if ((stringContains(ans,charToString('y')))||(stringContains(ans,charToString('Y'))))
        initial = "";


    if ((stringContains(ans,charToString('n')))||(stringContains(ans,charToString('N'))))
        initial = getLine("Type the 16 letters to appear on the board: ");

//check whether it is a valid input
    else
         cout << "Please type a word that begins with 'y' or 'n'." << endl;  //while loop

//check whether it is a valid 16-letter board
    if ((ans.length()) == 16)//should be initial.length(), while loop

        cout << "That is not a valid 16-letter board string. Try again." << endl;

//constructs a new Boggle
    Boggle boggle(dictionary, initial);
    clearConsole();
    initialize(4, 4);

    cout << "It's your turn" << endl;
    cout << boggle << endl;
    resetGUI(boggle);

    while (true)
    {
        string input = getLine("Type a word (or Enter to stop): ");

//resetGUI each time
        resetGUI(boggle);

        if (input.length() == 0)
            break;
        if (boggle.humanWordSearch(input))
        {
            cout << boggle << endl;
            setAnimationDelay(50);
            cout << "ok" << endl;
         }
        else
            cout << "You must enter an unfound 4+letter word from the dictionary" << endl;
    }

    Set<string> ret = boggle.computerWordSearch();
    cout << "It's my turn!" << endl;
    cout << "My words (" << ret.size() << "): {";
    //loop over each word's output
    for (string s : ret) {
    cout << "\"" << s << "\", ";
    cout << "}" << endl;
    cout << "My score: " << boggle.getScoreComputer() << endl;
    cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
}
}
