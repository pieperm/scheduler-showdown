#ifndef HIGHEST_RESPONSE_RATIO_NEXT_H
#define HIGHEST_RESPONSE_RATIO_NEXT_H

#include "../scheduler.h"

/**
 * Scheduler that implements the Highest Response Ratio Next (HRRN) algorithm
 * HRRN is a non-preemptive scheduling algorithm
 */
class HighestResponseRatioNext: public Scheduler {

public:

    /**
     * Chooses the process with the highest response ratio
     * Ratio = (time spent waiting + expected service time) / (expected service time)
     * @param currentProcess The index of the current process, or -1 if the CPU is free
     * @param currentTime The total time accumulated by the CPU so far
     * @param processList The list of processes for the scheduler to choose from
     * @return Index of the process in the list that should run at the next time step
     */
    int run(int currentProcess, int currentTime, vector<Process*>& processList) {

        int index = currentProcess;

        // HRRN is a non-preemptive algorithm, so do not choose a new process unless
        // there is no currently running process or the current process just finished
        if(currentProcess < 0 || processList.at(currentProcess)->isDone) {
            // find the process with the greatest ratio
            int maxRatio = -1;
            for(int i = 0; i < processList.size(); i++) {
                // only consider processes that have arrived at the CPU
                if (currentTime >= processList.at(i)->arrivalTime) {
                    // only consider processes that are not done
                    if (!processList.at(i)->isDone) {

                        // Compute the ratio for the process
                        // Ratio = (time spent waiting + expected service time) / (expected service time)
                        double timeWaiting = (currentTime - processList.at(i)->arrivalTime) - processList.at(i)->timeScheduled;
                        double expectedTime = processList.at(i)->totalTimeNeeded;
                        double ratio = (timeWaiting + expectedTime) / expectedTime;

                        // choose this process if it is higher than the maximum found so far
                        if(ratio > maxRatio) {
                            index = i;  // select the process
                            maxRatio = ratio;  // set the new maximum
                        }
                    }
                }
            }
        }

        return index;
    }

};

#endif //HIGHEST_RESPONSE_RATIO_NEXT_H
