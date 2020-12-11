#ifndef SHORTEST_PROCESS_NEXT_H
#define SHORTEST_PROCESS_NEXT_H

#include <vector>

#include "../scheduler.h"

using namespace std;

/**
 * Scheduler that implements the Shortest Process Next (SPN) algorithm
 * SPN is a non-preemptive scheduling algorithm
 */
class ShortestProcessNext: public Scheduler {

public:

    /**
     * Chooses the process with the shortest required computation time
     * @param currentProcess The index of the current process, or -1 if the CPU is free
     * @param currentTime The total time accumulated by the CPU so far
     * @param processList The list of processes for the scheduler to choose from
     * @return Index of the process in the list that should run at the next time step
     */
    int run(int currentProcess, int currentTime, vector<Process*>& processList) {

        int index = currentProcess;

        // SPN is a non-preemptive algorithm, so do not choose a new process unless
        // there is no currently running process or the current process just finished
        if(currentProcess < 0 || processList.at(currentProcess)->isDone) {

            int minTime = INFINITY;
            for(int i = 0; i < processList.size(); i++) {

                // only consider processes that have arrived at the CPU
                if(processList.at(i)->hasArrived) {
                    // only consider processes that are not done
                    if(!processList.at(i)->isDone) {
                        int totalTimeNeeded = processList.at(i)->totalTimeNeeded;
                        if(totalTimeNeeded < minTime) {
                            index = i;  // select this process
                            minTime = totalTimeNeeded;  // set the new minimum
                        }
                    }
                }
            }
        }

        return index;
    }

};

#endif //SHORTEST_PROCESS_NEXT_H
