#ifndef FIRST_IN_FIRST_OUT_H
#define FIRST_IN_FIRST_OUT_H

#include <deque>

#include "../scheduler.h"

using namespace std;

/**
 * Scheduler that implements the First-In First-Out (FIFO) algorithm
 * FIFO is a non-preemptive scheduling algorithm
 */
class FirstInFirstOut: public Scheduler {

public:

    deque<int> readyProcesses;


    /**
     * Chooses the first process that arrives at the CPU
     * @param currentProcess The index of the current process, or -1 if the CPU is free
     * @param currentTime The total time accumulated by the CPU so far
     * @param processList The list of processes for the scheduler to choose from
     * @return Index of the process in the list that should run at the next time step
     */
    int run(int currentProcess, int currentTime, vector<Process*>& processList) {

        int index = -1;

        for(unsigned int i = 0; i < processList.size(); i++) {
            // insert newly arrived processes that are not done into the queue
            if(processList.at(i)->arrivalTime == currentTime && !processList.at(i)->isDone) {
                readyProcesses.push_back(i);
            }
        }

        // Select a new process if the CPU is free and there are processes ready to run
        if(currentProcess < 0 && !readyProcesses.empty()) {
            // index of the next process to run is stored at the front of queue
            index = readyProcesses.front();
            readyProcesses.pop_front();
        } else if(currentProcess >= 0 && processList.at(currentProcess)->isDone) {
            // Select a new process if the previous one finishes
            index = readyProcesses.front();
            readyProcesses.pop_front();
        } else {  // otherwise, continue until the current process is finished
            index = currentProcess;
        }


        return index;
    }

};

#endif //FIRST_IN_FIRST_OUT_H
