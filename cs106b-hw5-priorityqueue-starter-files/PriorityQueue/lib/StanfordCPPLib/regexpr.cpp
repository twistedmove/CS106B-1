/*
 * File: regexpr.cpp
 * -----------------
 * Implementation of the functions in regexpr.h.
 * See regexpr.h for documentation of each function.
 */

#include "regexpr.h"
#include "platform.h"
using namespace std;

static Platform *pp = getPlatform();

bool regexMatch(string s, string regexp) {
    return pp->regexMatch(s, regexp);
}

int regexMatchCount(string s, string regexp) {
    return pp->regexMatchCount(s, regexp);
}

string regexReplace(string s, string regexp, string replacement, int limit) {
    return pp->regexReplace(s, regexp, replacement, limit);
}

namespace autograder {
int regexMatchCountWithLines(string s, string regexp, string& linesOut) {
    return pp->regexMatchCountWithLines(s, regexp, linesOut);
}
}
