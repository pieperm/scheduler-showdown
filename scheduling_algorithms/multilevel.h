#ifndef MULTILEVEL_H
#define MULTILEVEL_H

#include <vector>
#include <map>

#include "../scheduler.h"
#include "../interactive_process.h"
#include "round_robin.h"

using namespace std;

const int ML_NUM_LEVELS = 5;
const int ML_TIME_QUANTUM = 10;

/**
 * Data structure for a priority level for Multilevel
 */
struct Level {
    int priority = 0;  // the level's relative priority
    vector<Process*> processes;  // the processes in the level
    RoundRobin* roundRobinScheduler;  // RR for scheduling within the level
};

/**
 * Scheduler that implements the Multilevel (ML) algorithm
 * ML is a preemptive scheduling algorithm
 */
class Multilevel: public Scheduler {

public:

    map<int, Level> levels;  // maps priority to the corresponding level
    int timeQuantum = 0;

    /**
     * Constructor for a Multilevel scheduler
     * @param processList The list of processes
     */
    Multilevel(vector<Process*>& processList) {
        timeQuantum = ML_TIME_QUANTUM;

        // initialize data structures for levels
        for(int i = 1; i <= ML_NUM_LEVELS; i++) {
            Level newLevel;
            newLevel.priority = i;
            newLevel.roundRobinScheduler = new RoundRobin;
            levels.insert(pair<int, Level>(i, newLevel));  // add level to the map
        }

        // sort processes into different priority levels
        for(int i = 0; i < processList.size(); i++) {
            InteractiveProcess* process = reinterpret_cast<InteractiveProcess*>(processList.at(i));
            // clamp priorities that are too low or too high
            if(process->priority < 1) {
                process->priority = 1;
            } else if(process->priority > ML_NUM_LEVELS) {
                process->priority = ML_NUM_LEVELS;
            }
            // insert the process into the corresponding level
            levels.at(process->priority).processes.push_back(processList.at(i));
        }
    }

    /**
     * Chooses the process at the highest priority level using Round Robin scheduling
     * @param currentProcess The index of the current process, or -1 if the CPU is free
     * @param currentTime The total time accumulated by the CPU so far
     * @param processList The list of processes for the scheduler to choose from
     * @return Index of the process in the list that should run at the next time step
     */
    int run(int currentProcess, int currentTime, vector<Process*>& processList) {

        int index = -1;

        // For every level, update which processes have arrived at the CPU
        for(int p = ML_NUM_LEVELS; p > 0; p--) {
            vector<Process*> priorityProcesses = levels.at(p).processes;
            levels.at(p).roundRobinScheduler->updateReadyProcesses(currentTime, priorityProcesses);
        }

        // Start at the highest priority level
        for(int p = ML_NUM_LEVELS; p > 0; p--) {

            // Retrieve the list of processes for this level
            vector<Process*> priorityProcesses = levels.at(p).processes;

            // Run round robin scheduling on the current priority level
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
                break;  // do not continue searching for processes in lower priority levels
            }
        }

        return index;
    }

};

#endif //MULTILEVEL_H
