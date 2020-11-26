#ifndef ROUND_ROBIN_H
#define ROUND_ROBIN_H

#include <deque>
#include <iostream>

#include "../scheduler.h"

using namespace std;

class RoundRobin: public Scheduler {

private:
    int timeToNextSched = 0;
    deque<int> readyProcesses;

protected:
    void run() {
        cout << "Round Robin started" << endl;
    }

public:
    RoundRobin(int timeQuantum) {
        timeToNextSched = timeQuantum;
    }

};

#endif //ROUND_ROBIN_H
