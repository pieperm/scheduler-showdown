#ifndef ROUND_ROBIN_H
#define ROUND_ROBIN_H

#include <deque>
#include <iostream>
#include <vector>

#include "../scheduler.h"
#include "../process.h"

using namespace std;

int quantum = 10;

/**
 * Scheduler that implements the Round Robin (RR) algorithm
 * RR is a preemptive scheduling algorithm
 * Round Robin's functionality is split into two helper functions to make it compatible when used in
 * Multilevel scheduling and Multilevel Feedback scheduling
 */
class RoundRobin: public Scheduler {

public:

    int timeQuantum = 0;
    int timeToNextSched = 0;
    deque<int> readyProcesses;

    RoundRobin() {
        timeQuantum = quantum;
        timeToNextSched = quantum;
    }

    RoundRobin(int quantum) {
        timeQuantum = quantum;
    }

    /**
     * Chooses the next process that is ready and runs it for a set time quantum before moving to the next
     * @param currentProcess The index of the current process, or -1 if the CPU is free
     * @param currentTime The total time accumulated by the CPU so far
     * @param processList The list of processes for the scheduler to choose from
     * @return Index of the process in the list that should run at the next time step
     */
    int run(int currentProcess, int currentTime, vector<Process*>& processList) {

        int index = -1;

        updateReadyProcesses(currentTime, processList);
        index = getNextProcess(processList);

        return index;

    }

    /**
     * Helper function that updates the queue that tracks the order in which processes arrive at the CPU
     * @param currentTime The total time accumulated by the CPU so far
     * @param processList The list of processes for Round Robin to choose from
     */
    void updateReadyProcesses(int currentTime, vector<Process*>& processList) {
        for(int i = 0; i < processList.size(); i++) {
            if(processList.at(i)->arrivalTime == currentTime) {
                readyProcesses.push_back(i);
            }
        }
    }

    /**
     * Helper function that determines the next process to run
     * @param processList The list of processes for Round Robin to choose from
     * @return Index of the process in the list that should run at the next time step
     */
    int getNextProcess(vector<Process*>& processList) {

        int index = -1;

        // Check if the end of the time quantum was reached or if the process finished
        if(readyProcesses.size() > 0 && (timeToNextSched == 0 || processList[readyProcesses[0]]->isDone)) {
            // If the time quantum is over but the process isn't done, push it to the back of the queue
            if(!processList[readyProcesses[0]]->isDone) {
                readyProcesses.push_back(readyProcesses[0]);
            }

            readyProcesses.pop_front();  // move on to the next process in the queue
            timeToNextSched = timeQuantum;  // reset the time until the end of the next quantum
        }

        if(readyProcesses.size() > 0) {
            index = readyProcesses[0];  // the currently running process is at the head of the queue
            timeToNextSched--;  // decrement the amount of time before the end of the current time quantum
        } else {
            index = -1;  // no process to run
            timeToNextSched = 0;
        }

        return index;
    }

};

#endif //ROUND_ROBIN_H
