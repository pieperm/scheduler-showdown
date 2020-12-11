#ifndef MULTILEVEL_FEEDBACK_H
#define MULTILEVEL_FEEDBACK_H

#include <vector>
#include <map>
#include <cmath>

#include "../scheduler.h"
#include "../interactive_process.h"
#include "round_robin.h"

using namespace std;

const int MLF_NUM_LEVELS = 5;
const int MLF_TIME_QUANTUM = 10;

struct FeedbackLevel {
    int priority = 0;  // the level's relative priority
    vector<Process*> processes;  // the processes in the level
    int timeQuantum = MLF_TIME_QUANTUM;  // each level has its own time quantum
    RoundRobin* roundRobinScheduler;  // RR for scheduling within the level
};

/**
 * Scheduler that implements the Multilevel Feedback (MLF) algorithm
 * MLF is a preemptive scheduling algorithm
 */
class MultilevelFeedback: public Scheduler {

public:

    map<int, FeedbackLevel> levels;  // maps priority to the corresponding level

    /**
     * Constructor for a Multilevel Feedback scheduler
     * @param processList The list of processes
     */
    MultilevelFeedback(vector<Process*>& processList) {

        // initialize data structures for levels
        for(int i = 1; i <= MLF_NUM_LEVELS; i++) {
            FeedbackLevel newLevel;
            newLevel.priority = i;
            // Compute the time quantum for the priority level
            // Lower levels have exponentially higher time quanta
            newLevel.timeQuantum = pow(2, i - 1) * MLF_TIME_QUANTUM;
            // Initialize the level's Round Robin with the calculated time quantum
            newLevel.roundRobinScheduler = new RoundRobin(newLevel.timeQuantum);
            levels.insert(pair<int, FeedbackLevel>(i, newLevel));  // add level to the map
        }

        // sort processes into different priority levels
        for(int i = 0; i < processList.size(); i++) {
            InteractiveProcess* process = reinterpret_cast<InteractiveProcess*>(processList.at(i));
            // clamp priorities that are too low or too high
            if(process->priority < 1) {
                process->priority = 1;
            } else if(process->priority > MLF_NUM_LEVELS) {
                process->priority = MLF_NUM_LEVELS;
            }

            // insert the process into the corresponding level
            levels.at(process->priority).processes.push_back(processList.at(i));
        }
    }

    /**
     * Chooses the process at the highest priority level using Round Robin scheduling
     * Sends processes to lower priority levels as they surpass their respective time quanta
     * @param currentProcess The index of the current process, or -1 if the CPU is free
     * @param currentTime The total time accumulated by the CPU so far
     * @param processList The list of processes for the scheduler to choose from
     * @return Index of the process in the list that should run at the next time step
     */
    int run(int currentProcess, int currentTime, vector<Process*>& processList) {

        int index = -1;

        // For every level, update which processes have arrived at the CPU
        for(int p = MLF_NUM_LEVELS; p > 0; p--) {
            vector<Process*> priorityProcesses = levels.at(p).processes;
            levels.at(p).roundRobinScheduler->updateReadyProcesses(currentTime, priorityProcesses);
        }

        // Start at the highest priority level
        for(int p = MLF_NUM_LEVELS; p > 0; p--) {

            vector<Process*> priorityProcesses = levels.at(p).processes;

            // Run round robin scheduling on the current priority level with the corresponding time quantum
            int roundRobinResult = levels.at(p).roundRobinScheduler->getNextProcess(priorityProcesses);

            if(roundRobinResult >= 0) {
                // Get the id of the process that round robin returned
                string roundRobinId = priorityProcesses.at(roundRobinResult)->id;
                // Find the corresponding id in the actual process array to get the real index
                for(int i = 0; i < processList.size(); i++) {
                    if(roundRobinId == processList.at(i)->id) {
                        index = i;  // select index without being relative to a priority level
                    }
                }
                break;  // do not continue to lower priority levels
            }
        }

        return index;
    }

};

#endif //MULTILEVEL_FEEDBACK_H
