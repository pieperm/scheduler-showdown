#ifndef EARLIEST_DEADLINE_FIRST_H
#define EARLIEST_DEADLINE_FIRST_H

#include "../scheduler.h"
#include "../process.h"
#include "../realtime_process.h"

/**
 * Scheduler that implements the Earliest Deadline First (EDF) algorithm
 * EDF is a preemptive scheduling algorithm
 */
class EarliestDeadlineFirst: public Scheduler {

public:

    /**
     * Chooses the process with the shortest amount of time before its deadline
     * @param currentProcess The index of the current process, or -1 if the CPU is free
     * @param currentTime The total time accumulated by the CPU so far
     * @param processList The list of processes for the scheduler to choose from
     * @return Index of the process in the list that should run at the next time step
     */
    int run(int currentProcess, int currentTime, vector<Process*>& processList) {

        int index = -1;
        int minDeadline = 9999999;

        for(unsigned int i = 0; i < processList.size(); i++) {

            RealtimeProcess* process = reinterpret_cast<RealtimeProcess*>(processList.at(i));
            // only consider processes that have arrived and are not done
            if(process->hasArrived && !process->isDone) {
                // Get the process with the minimum deadline
                if(process->deadline < minDeadline) {
                    index = i;  // select the process
                    minDeadline = process->deadline;  // update the minimum
                }
            }
        }

        return index;

    }

};

#endif //EARLIEST_DEADLINE_FIRST_H
