#ifndef SCHEDULER_H
#define SCHEDULER_H

#define INFINITY 1000000

#include <vector>

#include "process.h"

using namespace std;

/**
 * Abstract class for scheduling algorithms to inherit from
 */
class Scheduler {

public:

    explicit Scheduler() {}

    /**
     * Pure virtual method for scheduling algorithms to implement
     * Chooses the next process to run by selecting an index from the list of processes
     * This function is executed once per time step
     * @param currentProcess The index of the current process, or -1 if the CPU is free
     * @param currentTime The total time accumulated by the CPU so far
     * @param processList The list of processes for the scheduler to choose from
     * @return Index of the process in the list that should run at the next time step
     */
    virtual int run(int currentProcess, int currentTime, vector<Process*>& processList) = 0;

};

#endif //SCHEDULER_H
