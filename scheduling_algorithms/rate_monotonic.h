#ifndef RATE_MONOTONIC_H
#define RATE_MONOTONIC_H

#include "../scheduler.h"
#include "../process.h"
#include "../realtime_process.h"

/**
 * Scheduler that implements the Rate Monotonic (RM) algorithm
 * RM is a preemptive scheduling algorithm
 */
class RateMonotonic: public Scheduler {

public:

    /**
     * Chooses the process with the lowest period
     * @param currentProcess The index of the current process, or -1 if the CPU is free
     * @param currentTime The total time accumulated by the CPU so far
     * @param processList The list of processes for the scheduler to choose from
     * @return Index of the process in the list that should run at the next time step
     */
    int run(int currentProcess, int currentTime, vector<Process*>& processList) {

        int index = -1;
        int minPeriod = 9999999;

        for(unsigned int i = 0; i < processList.size(); i++) {

            RealtimeProcess* process = reinterpret_cast<RealtimeProcess*>(processList.at(i));
            // only consider processes that have arrived and are not done
            if(process->hasArrived && !process->isDone) {
                // find process with the minimum period
                if(process->period < minPeriod) {
                    index = i;
                    minPeriod = process->period;
                }
            }

        }

        return index;

    }

};

#endif //RATE_MONOTONIC_H
