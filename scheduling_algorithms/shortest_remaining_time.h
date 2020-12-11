#ifndef SHORTEST_REMAINING_TIME_H
#define SHORTEST_REMAINING_TIME_H

#include "../scheduler.h"

/**
 * Scheduler that implements the Shortest Remaining Time (SRT) algorithm
 * SRT is a preemptive scheduling algorithm
 */
class ShortestRemainingTime: public Scheduler {

public:

    /**
     * Chooses the process with the least amount of time left to compute
     * @param currentProcess The index of the current process, or -1 if the CPU is free
     * @param currentTime The total time accumulated by the CPU so far
     * @param processList The list of processes for the scheduler to choose from
     * @return Index of the process in the list that should run at the next time step
     */
    int run(int currentProcess, int currentTime, vector<Process*>& processList) {

        int index = currentProcess;

        // SRT is a preemptive algorithm; no need to check if a process is already running
        int minTime = INFINITY;
        for(int i = 0; i < processList.size(); i++) {
            // only consider processes that have arrived at the CPU
            if(processList.at(i)->hasArrived) {
                // only consider processes that are not done
                if (!processList.at(i)->isDone) {
                    // compute the remaining time for this process: total time needed - time scheduled
                    int remainingTime = processList.at(i)->totalTimeNeeded - processList.at(i)->timeScheduled;
                    if (remainingTime < minTime) {
                        index = i;  // select the process
                        minTime = remainingTime;  // set the new minimum
                    }
                }
            }
        }

        return index;
    }

};

#endif //SHORTEST_REMAINING_TIME_H
