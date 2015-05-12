/*
 * File: dawglexicon.cpp
 * -----------------
 * A lexicon is a word list. This lexicon is backed by two separate data
 * structures for storing the words in the list:
 *
 * 1) a DAWG (directed acyclic word graph)
 * 2) a Set<string> of other words.
 *
 * Typically the DAWG is used for a large list read from a file in binary
 * format.  The STL set is for words added piecemeal at runtime.
 *
 * The DAWG idea comes from an article by Appel & Jacobson, CACM May 1988.
 * This lexicon implementation only has the code to load/search the DAWG.
 * The DAWG builder code is quite a bit more intricate, see me (Julie)
 * if you need it.
 */

#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <stdint.h>
#include <sstream>
#include "error.h"
#include "dawglexicon.h"
#include "strlib.h"
using namespace std;

static void toLowerCaseInPlace(string & str);

/*
 * The DAWG is stored as an array of edges. Each edge is represented by
 * one 32-bit struct.  The 5 "letter" bits indicate the character on this
 * transition (expressed as integer from 1 to 26), the  "accept" bit indicates
 * if you accept after appending that char (current path forms word), and the
 * "lastEdge" bit marks this as the last edge in a sequence of childeren.
 * The bulk of the bits (24) are used for the index within the edge array for
 * the children of this node. The children are laid out contiguously in
 * alphabetical order.  Since we read edges as binary bits from a file in
 * a big-endian format, we have to swap the struct order for little-endian
 * machines.
 */

DawgLexicon::DawgLexicon() {
    edges = start = NULL;
    numEdges = numDawgWords = 0;
}

DawgLexicon::DawgLexicon(string filename) {
    edges = start = NULL;
    numEdges = numDawgWords = 0;
    addWordsFromFile(filename);
}

DawgLexicon::~DawgLexicon() {
    if (edges) delete[] edges;
}

/*
 * Swaps a 4-byte long from big to little endian byte order
 */

static uint32_t my_ntohl(uint32_t arg) {
    uint32_t result = ((arg & 0xff000000) >> 24) |
            ((arg & 0x00ff0000) >> 8) |
            ((arg & 0x0000ff00) << 8) |
            ((arg & 0x000000ff) << 24);
    return result;
}

/*
 * Implementation notes: readBinaryFile
 * ------------------------------------
 * The binary lexicon file format must follow this pattern:
 * DAWG:<startnode index>:<num bytes>:<num bytes block of edge data>
 */

void DawgLexicon::readBinaryFile(string filename) {
    long startIndex, numBytes;
    char firstFour[4], expected[] = "DAWG";
    ifstream istr(filename.c_str(), __IOS_IN__ | __IOS_BINARY__);
    if (false) my_ntohl(0);
    if (istr.fail()) {
        error("DawgLexicon::addWordsFromFile: Couldn't open lexicon file " + filename);
    }
    istr.read(firstFour, 4);
    istr.get();
    istr >> startIndex;
    istr.get();
    istr >> numBytes;
    istr.get();
    if (istr.fail() || strncmp(firstFour, expected, 4) != 0
            || startIndex < 0 || numBytes < 0) {
        error("DawgLexicon::addWordsFromFile: Improperly formed lexicon file " + filename);
    }
    numEdges = numBytes/sizeof(Edge);
    edges = new Edge[numEdges];
    start = &edges[startIndex];
    istr.read((char *)edges, numBytes);
    if (istr.fail() && !istr.eof()) {
        error("DawgLexicon::addWordsFromFile: Improperly formed lexicon file " + filename);
    }

#if defined(BYTE_ORDER) && BYTE_ORDER == LITTLE_ENDIAN
    uint32_t *cur = (uint32_t *) edges;
    for (int i = 0; i < numEdges; i++, cur++) {
        *cur = my_ntohl(*cur);
    }
#endif

    istr.close();
    numDawgWords = countDawgWords(start);
}

int DawgLexicon::countDawgWords(Edge *ep) const {
    int count = 0;
    while (true) {
        if (ep->accept) count++;
        if (ep->children != 0) {
            count += countDawgWords(&edges[ep->children]);
        }
        if (ep->lastEdge) break;
        ep++;
    }
    return count;
}

/*
 * Check for DAWG in first 4 to identify as special binary format,
 * otherwise assume ASCII, one word per line
 */

void DawgLexicon::addWordsFromFile(string filename) {
    char firstFour[4], expected[] = "DAWG";
    ifstream istr(filename.c_str());
    if (istr.fail()) {
        error("DawgLexicon::addWordsFromFile: Couldn't open lexicon file " + filename);
    }
    istr.read(firstFour, 4);
    if (strncmp(firstFour, expected, 4) == 0) {
        if (otherWords.size() != 0) {
            error("DawgLexicon::addWordsFromFile: Binary files require an empty lexicon");
        }
        readBinaryFile(filename);
        return;
    }
    istr.seekg(0);
    string line;
    while (getline(istr, line)) {
        add(line);
    }
    istr.close();
}

int DawgLexicon::size() const {
    return numDawgWords + otherWords.size();
}

bool DawgLexicon::isEmpty() const {
    return size() == 0;
}

void DawgLexicon::clear() {
    if (edges) delete[] edges;
    edges = start = NULL;
    numEdges = numDawgWords = 0;
    otherWords.clear();
}

/*
 * Implementation notes: findEdgeForChar
 * -------------------------------------
 * Iterate over sequence of children to find one that
 * matches the given char.  Returns NULL if we get to
 * last child without finding a match (thus no such
 * child edge exists).
 */

DawgLexicon::Edge *DawgLexicon::findEdgeForChar(Edge *children, char ch) const {
    Edge *curEdge = children;
    while (true) {
        if (curEdge->letter == charToOrd(ch)) return curEdge;
        if (curEdge->lastEdge) return NULL;
        curEdge++;
    }
}

/*
 * Implementation notes: traceToLastEdge
 * -------------------------------------
 * Given a string, trace out path through the DAWG edge-by-edge.
 * If a path exists, return last edge; otherwise return NULL.
 */

DawgLexicon::Edge *DawgLexicon::traceToLastEdge(const string & s) const {
    if (!start) return NULL;
    Edge *curEdge = findEdgeForChar(start, s[0]);
    int len = (int) s.length();
    for (int i = 1; i < len; i++) {
        if (!curEdge || !curEdge->children) return NULL;
        curEdge = findEdgeForChar(&edges[curEdge->children], s[i]);
    }
    return curEdge;
}

bool DawgLexicon::containsPrefix(string prefix) const {
    if (prefix.empty()) return true;
    toLowerCaseInPlace(prefix);
    if (traceToLastEdge(prefix)) return true;
    __foreach__ (string word __in__ otherWords) {
        if (startsWith(word, prefix)) return true;
        if (prefix < word) return false;
    }
    return false;
}

bool DawgLexicon::contains(string word) const {
    toLowerCaseInPlace(word);
    Edge *lastEdge = traceToLastEdge(word);
    if (lastEdge && lastEdge->accept) return true;
    return otherWords.contains(word);
}

void DawgLexicon::add(string word) {
    toLowerCaseInPlace(word);
    if (!contains(word)) {
        otherWords.add(word);
    }
}

DawgLexicon::DawgLexicon(const DawgLexicon & src) {
    deepCopy(src);
}

DawgLexicon & DawgLexicon::operator=(const DawgLexicon & src) {
    if (this != &src) {
        if (edges != NULL) delete[] edges;
        deepCopy(src);
    }
    return *this;
}

void DawgLexicon::deepCopy(const DawgLexicon & src) {
    if (src.edges == NULL) {
        edges = NULL;
        start = NULL;
    } else {
        numEdges = src.numEdges;
        edges = new Edge[src.numEdges];
        memcpy(edges, src.edges, sizeof(Edge)*src.numEdges);
        start = edges + (src.start - src.edges);
    }
    numDawgWords = src.numDawgWords;
    otherWords = src.otherWords;
}

void DawgLexicon::mapAll(void (*fn)(string)) const {
    __foreach__ (string word __in__ *this) {
        fn(word);
    }
}

void DawgLexicon::mapAll(void (*fn)(const string &)) const {
    __foreach__ (string word __in__ *this) {
        fn(word);
    }
}

string DawgLexicon::toString() const {
    ostringstream out;
    out << *this;
    return out.str();
}

void DawgLexicon::iterator::advanceToNextWordInSet() {
    if (setIterator == setEnd) {
        currentSetWord = "";
    } else {
        currentSetWord = *setIterator;
        ++setIterator;
    }
}

void DawgLexicon::iterator::advanceToNextWordInDawg() {
    if (edgePtr == NULL) {
        edgePtr = lp->start;
    } else {
        advanceToNextEdge();
    }
    while (edgePtr != NULL && !edgePtr->accept) {
        advanceToNextEdge();
    }
}

void DawgLexicon::iterator::advanceToNextEdge() {
    Edge *ep = edgePtr;
    if (ep->children == 0) {
        while (ep != NULL && ep->lastEdge) {
            if (stack.isEmpty()) {
                edgePtr = NULL;
                return;
            } else {
                ep = stack.pop();
                currentDawgPrefix.resize(currentDawgPrefix.length() - 1);
            }
        }
        edgePtr = ep + 1;
    } else {
        stack.push(ep);
        currentDawgPrefix.push_back(lp->ordToChar(ep->letter));
        edgePtr = &lp->edges[ep->children];
    }
};

static void toLowerCaseInPlace(string & str) {
    int nChars = str.length();
    for (int i = 0; i < nChars; i++) {
        str[i] = tolower(str[i]);
    }
}

std::ostream& operator<<(std::ostream& out, const DawgLexicon& lex) {
    out << "{";
    bool first = true;
    __foreach__ (string word __in__ lex) {
        if (first) {
            first = false;
        } else {
            out << ", ";
        }
        out << word;
    }
    out << "}";
    return out;
}
