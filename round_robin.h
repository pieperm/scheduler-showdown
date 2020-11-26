#ifndef ROUND_ROBIN_H
#define ROUND_ROBIN_H

#include <deque>
#include <iostream>

#include "schedulers.h"

using namespace std;

class RoundRobin: public Scheduler {

    int timeToNextSched = 0;
    deque<int> readyProcesses;

public:
    RoundRobin(int timeQuantum) {
        timeToNextSched = timeQuantum;
    }

protected:
    void run() {
        cout << "Round Robin started" << endl;
    }

};

#endif //ROUND_ROBIN_H
