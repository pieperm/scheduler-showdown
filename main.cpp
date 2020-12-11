#include <iostream>
#include <chrono>
#include <vector>
#include <thread>
#include <iomanip>
#include <fstream>

#include "main.h"
#include "scheduler.h"
#include "process.h"
#include "realtime_process.h"
#include "interactive_process.h"
#include "batch_process.h"

#include "scheduling_algorithms/first_in_first_out.h"
#include "scheduling_algorithms/shortest_process_next.h"
#include "scheduling_algorithms/shortest_remaining_time.h"
#include "scheduling_algorithms/highest_response_ratio_next.h"
#include "scheduling_algorithms/round_robin.h"
#include "scheduling_algorithms/multilevel.h"
#include "scheduling_algorithms/multilevel_feedback.h"
#include "scheduling_algorithms/rate_monotonic.h"
#include "scheduling_algorithms/earliest_deadline_first.h"

using namespace std;

/**
 * Runs the main program for Scheduler Showdown: Expansion
 * Prompts the user to select a process type and a scheduling algorithm
 * Outputs a table of the running processes and then run statistics
 * @param argc The number of provided command-line arguments
 * @param argv The list of provided command-line arguments
 * @return 0 if the run is successful
 */
int main(int argc, char* argv[]) {

    string filename;
    int currentTime = 0;
    chrono::duration<int, std::milli> sleepTime = chrono::milliseconds(500);
    vector<Process*> processList;
    int processIndex;
    bool allProcessesComplete;
    int timesIdle = 0;  // counter for the number of time steps when the CPU was idle

    // Take in command-line arguments
    // Format: process_file sleep_time
    if(argc == 1) {
        cerr << "Did not specify process file" << endl;
        exit(-1);
    } else if(argc == 2) {
        cerr << "Did not specify sleep time" << endl;
        exit(-1);
    } else {
        filename = argv[1];
        sleepTime = chrono::milliseconds(stoul(argv[2]));
    }

    cout << "----------------------------------------------" << endl;
    cout << "  Welcome to Scheduler Simulator: Expansion!  " << endl;
    cout << "----------------------------------------------" << endl << endl;

    // Prompt user to select the type of process
    ProcessType processType = promptForProcessType();

    // Read processes from file into a vector
    readProcessFile(filename, processType, processList);

    Scheduler* schedulingAlgorithm;
    // Prompt the user to select the scheduling algorithm for the selected process type
    int schedulerType = promptForSchedulingAlgorithm(processType);

    switch (processType) {
        case BATCH:
            switch(schedulerType) {
                case 1:  // FIFO
                {
                    FirstInFirstOut* fifo = new FirstInFirstOut;
                    schedulingAlgorithm = reinterpret_cast<Scheduler*>(fifo);
                    break;
                }
                case 2:  // SPN
                {
                    ShortestProcessNext* spn = new ShortestProcessNext;
                    schedulingAlgorithm = reinterpret_cast<Scheduler*>(spn);
                    break;
                }
                case 3:  // SRT
                {
                    ShortestRemainingTime* srt = new ShortestRemainingTime;
                    schedulingAlgorithm = reinterpret_cast<Scheduler*>(srt);
                    break;
                }
                case 4:  // HRRN
                {
                    HighestResponseRatioNext* hrrn = new HighestResponseRatioNext;
                    schedulingAlgorithm = reinterpret_cast<Scheduler*>(hrrn);
                    break;
                }
            }
            break;
        case INTERACTIVE:
            switch(schedulerType) {
                case 1:  // RR
                {
                    RoundRobin* rr = new RoundRobin;
                    schedulingAlgorithm = reinterpret_cast<Scheduler*>(rr);
                    break;
                }
                case 2:  // ML
                {
                    Multilevel* ml = new Multilevel(processList);
                    schedulingAlgorithm = reinterpret_cast<Scheduler*>(ml);
                    break;
                }
                case 3:  // MLF
                {
                    MultilevelFeedback* mlf = new MultilevelFeedback(processList);
                    schedulingAlgorithm = reinterpret_cast<Scheduler*>(mlf);
                    break;
                }
                case 4:  // FIFO
                {
                    FirstInFirstOut* fifo = new FirstInFirstOut;
                    schedulingAlgorithm = reinterpret_cast<Scheduler*>(fifo);
                    break;
                }
            }
            break;
        case REALTIME:
            switch(schedulerType) {
                case 1:  // RM
                {
                    RateMonotonic* rm = new RateMonotonic;
                    schedulingAlgorithm = reinterpret_cast<Scheduler*>(rm);
                    break;
                }
                case 2:  // EDF
                {
                    EarliestDeadlineFirst* edf = new EarliestDeadlineFirst;
                    schedulingAlgorithm = reinterpret_cast<Scheduler*>(edf);
                    break;
                }
                case 3:  // FIFO
                {
                    FirstInFirstOut* fifo = new FirstInFirstOut;
                    schedulingAlgorithm = reinterpret_cast<Scheduler*>(fifo);
                    break;
                }
            }
            break;
        default:
            cerr << "Invalid process type" << endl;
    }

    cout << endl << "Starting simulation..." << endl << endl;
    cout << "+--------------------------------------------------------------------------+" << endl;
    cout << "|                                   KEY                                    |" << endl;
    cout << "+--------------------------------------------------------------------------+" << endl;
    cout << "| The leftmost column indicates the current time                           |" << endl;
    cout << "| The middle columns correspond to one of the processes                    |" << endl;
    cout << "|     0: The process is running                                            |" << endl;
    cout << "|     blank: The process is not running                                    |" << endl;
    cout << "|     X: The process is running and finished at this time                  |" << endl;
    cout << "|     !: The process has run for longer than its required computation time |" << endl;
    cout << "| The rightmost column indicates whether the CPU is idle                   |" << endl;
    cout << "|     +: CPU is running a process                                          |" << endl;
    cout << "|     -: CPU is idle                                                       |" << endl;
    cout << "+--------------------------------------------------------------------------+" << endl;
    cout << endl << endl;

    cout << "Time |";
    for(int i = 0; i < processList.size(); i++) {
        if(i + 1 < 10) {
            cout << "  P" << i + 1 << "  |";
        } else {
            cout << "  P" << i + 1 << " |";
        }
    }
    cout << "  CPU |" << endl;

    cout << "-----";
    for(int i = 0; i < processList.size(); i++) {
        cout << "+------";
    }
    cout << "+------+" << endl;

    allProcessesComplete = false;  // set flag for when CPU can quit
    currentTime = 0;  // start out with a CPU time of 0
    processIndex = -1;  // start with no process running on the CPU

    while(!allProcessesComplete) {

        // Update whether process has arrived at the CPU based on the current time
        for(unsigned int i = 0; i < processList.size(); i++) {
            if(currentTime == processList.at(i)->arrivalTime) {
                processList.at(i)->hasArrived = true;
            }
        }

        // Run the selected scheduling algorithm at the current time
        processIndex = schedulingAlgorithm->run(processIndex, currentTime, processList);

        // If the returned index is valid, then update its member variables accordingly
        if(processIndex >= 0 && static_cast<unsigned int>(processIndex) < processList.size()) {
            processList[processIndex]->timeScheduled++;  // The process was scheduled for another time unit
            // Mark a process as done when its required computation time equals its time spent running on the CPU
            if(processList[processIndex]->totalTimeNeeded == processList[processIndex]->timeScheduled) {
                processList[processIndex]->isDone = true;
                processList[processIndex]->timeFinished = currentTime;
            }
        }

        // Output the row for the current clock cycle
        cout << setw(4) << currentTime;
        allProcessesComplete = true;
        for(unsigned int i = 0; i < processList.size(); i++) {

            // update whether all processes have completed
            allProcessesComplete = allProcessesComplete && processList[i]->isDone;

            if(i == processIndex) {
                if(processList[i]->isDone) {
                    if(processList[i]->timeScheduled > processList[i]->totalTimeNeeded) {
                        cout << " |   ! ";
                    }
                    else {
                        cout << " |   X ";
                    }
                } else {
                    cout << " |   O ";
                }
            } else {
                cout << " |     ";
            }
        }

        // Last column indicates CPU status
        if(processIndex < 0 || processIndex >= processList.size()) {
            cout << " |   - ";  // CPU is idle; no currently running process
            timesIdle++;
        } else {
            cout << " |   + ";  // CPU has a running process
        }
        cout << " |" << endl;

        if(!allProcessesComplete) {
            this_thread::sleep_for(sleepTime);  // delay between each clock cycle
            currentTime = currentTime + 1;  // update the current time
        }

    }

    // Once finished, output statistics about the run
    outputStatistics(processList, processType, timesIdle, currentTime);

    for(int i = 0; i < processList.size(); i++) {
        delete processList.at(i);
    }

    return 0;
}

/**
 * Read in data from a file and interpret it as a list of processes of a certain type
 * The first line of the file indicates the number of processes
 * Each following line represents a process with information such as the process id, arrival time,
 * required processing time, and additional data specific to the process type
 * @param fname The name of the file to read from
 * @param processType The type of processes in the given file
 * @param procList The list representation of all the processes
 * @post procList is filled with the processes from the file
 */
void readProcessFile(const string& fname, const ProcessType processType, vector<Process*>& procList) {

    ifstream in(fname.c_str());
    int numProcesses;

    if(in.fail()) {
        cerr << "Unable to open file \"" << fname << "\", terminating program" << endl;
        exit(-1);
    }

    in >> numProcesses;
    procList.resize(numProcesses);
    for(int i = 0; i < procList.capacity(); i++) {
        procList.at(i) = nullptr;
    }

    for(int i = 0; i < procList.capacity(); i++) {
        switch (processType) {
            case BATCH: {
                BatchProcess* batchProcess = new BatchProcess;
                in >> batchProcess->id
                   >> batchProcess->arrivalTime
                   >> batchProcess->totalTimeNeeded;
                procList.at(i) = batchProcess;
                break;
            }
            case INTERACTIVE: {
                InteractiveProcess* interactiveProcess = new InteractiveProcess;
                in >> interactiveProcess->id
                   >> interactiveProcess->arrivalTime
                   >> interactiveProcess->totalTimeNeeded
                   >> interactiveProcess->priority;
                procList.at(i) = interactiveProcess;
                break;
            }
            case REALTIME: {
                RealtimeProcess* realtimeProcess = new RealtimeProcess;
                in >> realtimeProcess->id
                   >> realtimeProcess->arrivalTime
                   >> realtimeProcess->totalTimeNeeded
                   >> realtimeProcess->period
                   >> realtimeProcess->deadline;
                procList.at(i) = realtimeProcess;
                break;
            }
            default: {
                cerr << "Invalid process type supplied: " << processType << endl;
                exit(-1);
            }
        }

    }

    in.close();
}

/**
 * Prompt the user to select the type of process to simulate
 * @return The selected ProcessType enum (BATCH, INTERACTIVE, or REALTIME)
 */
ProcessType promptForProcessType() {
    ProcessType processChoice = NONE;
    int input;

    do {
        cout << "What process type would you like to use?" << endl;
        cout << "\t1) Batch Processes" << endl;
        cout << "\t2) Interactive Processes" << endl;
        cout << "\t3) Realtime Processes" << endl;

        cout << "Enter the corresponding number: ";
        cin >> input;

        if(input > 0 && input <= 3) {
            processChoice = static_cast<ProcessType>(input);
        } else {
            cerr << "Invalid process type. You entered " << input << " but 1, 2, or 3 was expected." << endl;
        }

    } while (processChoice == NONE);

    return processChoice;
}

/**
 * Prompt the user to select a scheduling algorithm to use based on their choice of process type
 * Different algorithm options are presented based on whether the type is batch, interactive, or realtime
 * @param processType The user's selected process type (BATCH, INTERACTIVE, or REALTIME)
 * @return The number corresponding to the listed algorithm options
 */
int promptForSchedulingAlgorithm(ProcessType processType) {
    int schedulerChoice = -1;
    int input;

    do {
        switch (processType) {
            case BATCH:
                cout << endl;
                cout << "What scheduler would you like to use for batch processes?" << endl;
                cout << "\t1) First-In First-Out (FIFO)" << endl;
                cout << "\t2) Shortest Process Next (SPN)" << endl;
                cout << "\t3) Shortest Remaining Time (SRT)" << endl;
                cout << "\t4) Highest Response Ratio Next (HRRN)" << endl;
                cout << "Enter the corresponding number: ";
                cin >> input;

                if(input > 0 && input <= 4) {
                    schedulerChoice = input;
                } else {
                    cerr << "Invalid scheduler selected. You entered " << input << " but 1, 2, 3, or 4 was expected." << endl << endl;
                }

                break;
            case INTERACTIVE:
                cout << endl;
                cout << "What scheduler would you like to use for interactive processes?" << endl;
                cout << "\t1) Round Robin (RR)" << endl;
                cout << "\t2) Multilevel (ML)" << endl;
                cout << "\t3) Multilevel Feedback (MLF)" << endl;
                cout << "\t4) First-In First-Out (FIFO)" << endl;
                cout << "Enter the corresponding number: ";
                cin >> input;

                if(input > 0 && input <= 4) {
                    schedulerChoice = input;
                } else {
                    cerr << "Invalid scheduler selected. You entered " << input << " but 1, 2, 3, or 4 was expected." << endl << endl;
                }
                break;
            case REALTIME:
                cout << endl;
                cout << "What scheduler would you like to use for realtime processes?" << endl;
                cout << "\t1) Rate Monotonic (RM)" << endl;
                cout << "\t2) Earliest Deadline First (EDF)" << endl;
                cout << "\t3) First-In First-Out (FIFO)" << endl;
                cout << "Enter the corresponding number: ";
                cin >> input;

                if(input > 0 && input <= 3) {
                    schedulerChoice = input;
                } else {
                    cerr << "Invalid scheduler selected. You entered " << input << " but 1, 2, or 3 was expected." << endl << endl;
                }
                break;
            default:
                cerr << "Invalid argument processType; expected BATCH, INTERACTIVE, or REALTIME" << endl << endl;
        }
    } while (schedulerChoice == -1);

    return schedulerChoice;
}

/**
 * Print statistics about the simulation, including turnaround times, normalized turnaround times, amount of time
 * the processor was busy, and, for realtime processes, information about meeting deadlines
 * @param processList The list of processes
 * @param selectedProcessType The process type the user selected to simulate
 * @param timesIdle The number of clock cycles the CPU was idle during the simulation
 * @param currentTime The time at the end of the simulation
 */
void outputStatistics(vector<Process*>& processList, ProcessType selectedProcessType, int timesIdle, int currentTime) {

    cout << "\n\nRun Statistics:" << endl;

    // accumulators for the summations of turnaround times (for computing means)
    double turnaroundTimeSum = 0;
    double normalizedTurnaroundTimeSum = 0;

    // Output statistics in table format
    cout << "Process | Finish Time | Turnaround Time | Normalized Turnaround Time |" << endl;
    cout << "----------------------------------------------------------------------" << endl;

    // Output process id, finish time, turnaround time, and normalized turnaround time for each process
    for(int i = 0; i < processList.size(); i++) {

        // compute turnaround time and normalized turnaround time for the process
        int turnaroundTime = processList.at(i)->timeFinished - processList.at(i)->arrivalTime;
        double normalizedTurnaroundTime = turnaroundTime / static_cast<double>(processList.at(i)->timeScheduled);

        // update sums
        turnaroundTimeSum += turnaroundTime;
        normalizedTurnaroundTimeSum += normalizedTurnaroundTime;

        // output process statistics as a row in the table
        cout << setw(7) << processList.at(i)->id << " |";
        cout << setw(12) << processList.at(i)->timeFinished << " |";
        cout << setw(16) << turnaroundTime << " |";
        cout << setw(27) << fixed << setprecision(2) << normalizedTurnaroundTime << " |";
        cout << endl;
    }

    // compute the mean turnaround time and mean normalized turnaround time for all processes
    double meanTurnaroundTime = turnaroundTimeSum / processList.size();
    double meanNormalizedTurnaroundTime = normalizedTurnaroundTimeSum / processList.size();

    // output means as the final row in the table
    cout << "----------------------------------------------------------------------" << endl;
    cout << "   Mean |";
    cout << "             |";  // empty cell for finish time
    cout << setw(16) << fixed << setprecision(2) << meanTurnaroundTime << " |";
    cout << setw(27) << fixed << setprecision(2) << meanNormalizedTurnaroundTime << " |";
    cout << endl << endl;

    double idlePercentage = (timesIdle / (double)currentTime) * 100;
    cout << "The processor was idle for " << timesIdle << "/" << currentTime << " clock cycles (" << idlePercentage << "%)" << endl << endl;

    // For realtime processes, output how many met their deadlines
    if(selectedProcessType == REALTIME) {
        int metDeadlines = 0;
        for(int i = 0; i < processList.size(); i++) {
            RealtimeProcess* realtimeProcess = reinterpret_cast<RealtimeProcess*>(processList.at(i));

            if(realtimeProcess->timeFinished <= realtimeProcess->deadline) {
                metDeadlines++;
            }
        }

        double metDeadlinesPercentage = (metDeadlines / (double)processList.size()) * 100;
        cout << metDeadlinesPercentage << "% of processes met their deadlines." << endl;

        for(int i = 0; i < processList.size(); i++) {
            RealtimeProcess* realtimeProcess = reinterpret_cast<RealtimeProcess*>(processList.at(i));
            cout << "    "
                 << (realtimeProcess->timeFinished <= realtimeProcess->deadline ? "+" : "-")
                 << " P"
                 << i + 1
                 << " had a deadline of "
                 << realtimeProcess->deadline
                 << " and finished at "
                 << realtimeProcess->timeFinished
                 << endl;
        }
    }
}