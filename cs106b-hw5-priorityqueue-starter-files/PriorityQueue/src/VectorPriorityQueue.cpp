// This file contains the implementation of the priority queue using a vector as internal storage

#include "VectorPriorityQueue.h"


VectorPriorityQueue::VectorPriorityQueue() {

}


VectorPriorityQueue::~VectorPriorityQueue() {

}

void VectorPriorityQueue::changePriority(string value, int newPriority) {
    for (int i = 0; i < list.size(); i++)
        if (list[i].value == value)
            list[i].priority = newPriority;
}
//throw exception

void VectorPriorityQueue::clear() {
    list.clear();
}

//helper function to get the element with highest priority
int VectorPriorityQueue::besti() const {
    int ret = 0;
    for (int i = 1; i < size(); i++)
        if (list[i].priority < list[ret].priority)
            ret = i;
    return ret;
}

string VectorPriorityQueue::dequeue() {
    string ret = list[besti()].value;
    list.remove(besti());
    return ret;
}

void VectorPriorityQueue::enqueue(string value, int priority) {
    list.push_back(PQEntry(value, priority));
    cout << "Enqueued \"" << value << "\" with priority " << priority << endl;
}

bool VectorPriorityQueue::isEmpty() const {
    if(list.isEmpty())
        return true;
    else
        return false;
}

//return value of the helper function
string VectorPriorityQueue::peek() const {
    return list.get(besti()).value;
}

//return priority of the helper function
int VectorPriorityQueue::peekPriority() const {
    return list.get(besti()).priority;
}

int VectorPriorityQueue::size() const {
    return list.size();
}

ostream& operator<<(ostream& out, const VectorPriorityQueue& queue) {
    out << "{";
    for (int i = 0; i < queue.size(); i++) {
        out << "\"" << queue.list[i].value << "\":" << queue.list[i].priority;
        if (i != queue.size() - 1)
            out << ", ";
    }
    out << "}";
    return out;
}
