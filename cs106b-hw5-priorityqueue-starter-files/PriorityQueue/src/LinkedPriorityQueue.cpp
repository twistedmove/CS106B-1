//This file contains the implementation of the priority queue using a linked list as internal storage

#include "LinkedPriorityQueue.h"
#include <iostream>

using namespace std;

//constructor function
LinkedPriorityQueue::LinkedPriorityQueue() {
    front = NULL;
}

//destructor function
LinkedPriorityQueue::~LinkedPriorityQueue() {
    clear();
}

void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    ListNode* tmp = front;
    if (front->value == value){
        front = front->next;
        enqueue(value, newPriority);
    }else{
        while(tmp->next != NULL && tmp->next->value != value){
            tmp = tmp->next;
        }
        if(tmp->next != NULL ){
            tmp->next = tmp->next->next;
                   enqueue(value, newPriority);
             return;
        }else{
            return;
        }
    }
}


void LinkedPriorityQueue::clear() {
    while (front)
    {
        ListNode* tmp = front->next;
        delete front;
        front = tmp;
    }
}

string LinkedPriorityQueue::dequeue() {
    string ret = front->value;
    ListNode* tmp = front->next;
    delete front;
    front = tmp;
    return ret;
}

void LinkedPriorityQueue::enqueue(string value, int priority) {
    if (front == NULL)
        front = new ListNode(value, priority);
    else if (priority < front->priority)
        front = new ListNode(value, priority, front);
    else {
        ListNode* tmp = front;
        while((tmp->next != NULL) && (tmp->next->priority < priority || (tmp->next->priority == priority && tmp->next->value < value))){
            tmp = tmp->next;
        }

        tmp->next =  new ListNode(value, priority, tmp->next);

    }
    cout << "Enqueued \"" << value << "\" with priority " << priority << endl;
}



bool LinkedPriorityQueue::isEmpty() const {
    return front == NULL;
}

string LinkedPriorityQueue::peek() const {
    return front->value;
}

int LinkedPriorityQueue::peekPriority() const {
    return front->priority;
}

int LinkedPriorityQueue::size() const {
    int ret = 0;
    ListNode* tmp = front;
    while (tmp)
    {
        tmp = tmp->next;
        ++ret;
    }
    return ret;
}

ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    out << "{";
    ListNode* tmp = queue.front;
    while (tmp) {
        out << "\"" << tmp->value << "\":"<<tmp->priority;
        if (tmp->next)
            out << ", ";
        tmp = tmp->next;
    }
    out << "}";
    return out;
}
