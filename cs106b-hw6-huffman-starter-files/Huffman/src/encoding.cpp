#include "encoding.h"
#include "foreach.h"
#include "filelib.h"
#include <queue>
#include <vector>
#include <functional>

using namespace std;

Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;   // this is just a placeholder so it will compile
    char c;
    while ((c = input.get()) != -1) {
        if (!freqTable.containsKey(c))
            freqTable.put(c, 0);
        int count = freqTable.get(c);
        freqTable.put(c, count + 1); //count and return a mapping from each character
    }
    freqTable.put(PSEUDO_EOF, 1);
    return freqTable;          // this is just a placeholder so it will compile
}

class QueueNode {
public:
    HuffmanNode *tree;
    int timeStamp;

    QueueNode(HuffmanNode *node, int ts) {
        this->tree = node;
        this->timeStamp = ts;
    }

    bool operator<(const QueueNode &x) const {  //compare strings when count is the same
        if (tree->count != x.tree->count)
            return tree->count > x.tree->count;
        else
            return timeStamp > x.timeStamp;
    }
};


HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    priority_queue<QueueNode> q;
    Vector<int> keys = freqTable.keys();
    int ts = 0;
    for (int i = 0; i < keys.size(); i++) {
        int key = keys[i], value = freqTable.get(key);
        q.push(QueueNode(new HuffmanNode(key, value), ts++));
    }

    while (q.size() > 1) {
        QueueNode node1 = q.top();
        q.pop();
        QueueNode node2 = q.top();
        q.pop();

        int newFreq = node1.tree->count + node2.tree->count;  //setup children of the tree
        HuffmanNode* newNode = new HuffmanNode(NOT_A_CHAR, newFreq, node1.tree, node2.tree);
        q.push(QueueNode(newNode, ts++));
    }

    return q.top().tree;
}

void buildEncodingMapHelper(Map<int, string> &result, HuffmanNode* tree, string prefix) {  //helper function
    if (tree == NULL)
        return;
    if (tree->isLeaf()) {
        result.put(tree->character, prefix);
    } else {
        buildEncodingMapHelper(result, tree->zero, prefix + "0");
        buildEncodingMapHelper(result, tree->one, prefix + "1");
    }
}

Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;   // this is just a placeholder so it will compile
    if (encodingTree->isLeaf())
        encodingMap.put(encodingTree->character, "0");
    else
        buildEncodingMapHelper(encodingMap, encodingTree, string());
    return encodingMap;             // this is just a placeholder so it will compile
}

void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    rewindStream(input);
    char c;
    while ((c = input.get()) != -1) {
        string s = encodingMap.get(c);
        for (int i = 0; i < s.length(); ++i)
            output.writeBit(s[i] - '0');
    }
}

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    int bit;
    HuffmanNode *curr = encodingTree;
    while ((bit = input.readBit()) != -1) {
        if (bit == 0 && curr->zero != NULL)
            curr = curr->zero;
        else if (bit == 1 && curr->one != NULL)
            curr = curr->one;
        else if (bit == 0 && curr->zero == NULL) {
            output.put(curr->character);
            curr = encodingTree->zero;
        } else {
            output.put(curr->character);
            curr = encodingTree->one;
        }
    }
}

void compress(istream& input, obitstream& output) {
    Map<int, int> freqTable = buildFrequencyTable(input);
    output << freqTable;
    HuffmanNode* tree = buildEncodingTree(freqTable);
    Map<int, string> encodingMap = buildEncodingMap(tree);
    encodeData(input, encodingMap, output);
    printSideways(tree,false,"");
}

void decompress(ibitstream& input, ostream& output) {
    Map<int, int> freqTable;
    input >> freqTable;
    HuffmanNode* tree = buildEncodingTree(freqTable);
    decodeData(input, tree, output);
    HuffmanNode* encodingTree;
    cout << encodingTree <<endl;
    printSideways(tree,false,"");
}


void freeTree(HuffmanNode* node) {
    if (node == NULL)
        return;
    while (node) {
        freeTree(node->zero);
        freeTree(node->one);
        delete node;
    }
}
