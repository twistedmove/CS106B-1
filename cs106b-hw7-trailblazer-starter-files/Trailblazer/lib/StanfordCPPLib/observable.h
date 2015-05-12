/*
 *
 * Author: Marty Stepp
 * Version: 2014/03/04
 */

#ifndef _observable_h
#define _observable_h

#include <set>
#include "error.h"

class Observable;
class Observer;

class Observable {
public:
    void addObserver(Observer* obs);
    void notifyObservers(void* arg = NULL);
    void removeObserver(Observer* obs);

private:
    std::set<Observer*> m_observers;
};

class Observer {
public:
    virtual void update(Observable* obs, void* arg = NULL) = 0;
};

#endif
