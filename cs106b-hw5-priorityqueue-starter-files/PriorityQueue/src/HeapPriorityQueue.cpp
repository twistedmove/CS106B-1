// This file contains the implementation of the priority queue using a heap array as internal storage

#include "HeapPriorityQueue.h"

//constructor function
HeapPriorityQueue::HeapPriorityQueue() {
    capacity = 3;
    len = 0;
    data = new PQEntry[capacity];
}

//destructor function
HeapPriorityQueue::~HeapPriorityQueue() {
    delete[] data;
}

//get index of left child in binary tree
int HeapPriorityQueue::lchild(int i) const {
    return i * 2 + 1;
}

//get index of right child in binary tree
int HeapPriorityQueue::rchild(int i) const{
    return i * 2 + 2;
}

//get index of parent in binary tree
int HeapPriorityQueue::parent(int i )const {
    return (i - 1) / 2;
}

//helper function of bubbling down
void HeapPriorityQueue::down(int i) {
    if (lchild(i) >= len)
        return;
    else if (rchild(i) < len) {
        int t = min(data[lchild(i)].priority, data[rchild(i)].priority);
        if (data[i].priority > t) {
            if (data[lchild(i)].priority < data[rchild(i)].priority) {
                swap(data[i], data[lchild(i)]);
                down(lchild(i));
            }
            else if (data[lchild(i)].priority > data[rchild(i)].priority) {
                swap(data[i], data[rchild(i)]);
                down(rchild(i));
            }
         else {
                if (data[lchild(i)].value <= data[rchild(i)].value) {
                    swap(data[i], data[lchild(i)]);
                    down(lchild(i));
                }
                else {
                    swap(data[i], data[rchild(i)]);
                    down(rchild(i));
                }
            }
        }
    }
}

//helper function of bubbling up
void HeapPriorityQueue::up(int i){
    if (i == 0 || data[i].priority > data[parent(i)].priority ||(data[i].priority == data[parent(i)].priority) && (data[i].value > data[parent(i)].value))
        return;
    swap(data[i], data[parent(i)]);
    up(parent(i));
}

void HeapPriorityQueue::changePriority(string value, int newPriority) {
    for (int i = 0; i <len; i++)
        if (data[i].value == value) {
            if (newPriority < data[i].priority)
            {
                data[i].priority = newPriority;
                up(i);
            }
            else
            {
                data[i].priority = newPriority;
                down(i);
            }
            break;
        }
}

void HeapPriorityQueue::clear() {
    delete[] data;
    capacity = 3;
    len = 0;
    data = new PQEntry[capacity];
}

string HeapPriorityQueue::dequeue() {
    string ret = data[0].value;
    data[0] = data[len - 1];
    len--;
    down(0);
    return ret;
}

void HeapPriorityQueue::enqueue(string value, int priority) {
    if (len == capacity)
    {
        capacity *= 2;
        PQEntry* tmp = data;
        data = new PQEntry[capacity];
        for (int i = 0; i < len; i++)
            data[i] = tmp[i];
        delete[] tmp;
    }

    data[len] = PQEntry(value, priority);
    len++;
    up(len - 1);
    cout << "Enqueued \"" << value << "\" with priority " << priority << endl;
}

bool HeapPriorityQueue::isEmpty() const {
    return len == 0;
}

string HeapPriorityQueue::peek() const {
    return data[0].value;
}

int HeapPriorityQueue::peekPriority() const {
    return data[0].priority;
}

int HeapPriorityQueue::size() const {
    return len;
}

ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    out <<"{";
    for (int i = 0; i < queue.len; i++) {
        out << "\"" << queue.data[i].value << "\":" <<queue.data[i].priority;
        if (i != queue.len - 1)
            out <<", ";
    }
    out <<"}";
    return out;
}
