#ifndef REALTIME_PROCESS_H
#define REALTIME_PROCESS_H

#include "process.h"

/**
 * Class representing a realtime process
 * Child class of Process
 */
class RealtimeProcess: public Process {

public:

    int period;  // the period, or how often a process should run
    int deadline;  // the deadline, the CPU time the process must finish by

    /**
     * Constructor for a realtime process
     */
    RealtimeProcess() {
        processType = REALTIME;
    }

};

#endif //REALTIME_PROCESS_H
