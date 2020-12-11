#ifndef PROCESS_H
#define PROCESS_H

#include <string>

using namespace std;

enum ProcessType { NONE = 0, BATCH = 1, INTERACTIVE = 2, REALTIME = 3 };

/**
 * Abstract class for processes to inherit from
 * Contains information relevant to all types of processes
 * Child classes: BatchProcess, InteractiveProcess, and RealtimeProcess
 */
class Process {

public:

    explicit Process() {}

    string id = "";  // unique identifier for the process
    ProcessType processType;  // enum identifying the type of process
    bool hasArrived = false;  // flag whether the process has arrived at the CPU
    bool isDone = false;  // whether the process has finished computing
    int arrivalTime = -1;  // the time that the process arrives at the CPU (-1 if not arrived yet)
    int timeScheduled = 0;  // the amount of time the process has run on the CPU so far
    int totalTimeNeeded = 0;  // the total amount of time the process needs to run
    int timeFinished = -1;  // the time that the process finishes

};

#endif //PROCESS_H
