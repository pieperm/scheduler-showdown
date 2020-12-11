#ifndef INTERACTIVE_PROCESS_H
#define INTERACTIVE_PROCESS_H

#include "process.h"

using namespace std;

/**
 * Class representing an interactive process
 * Child class of Process
 */
class InteractiveProcess: public Process {

public:

    int priority;  // priority is unique to interactive processes

    /**
     * Constructor for an interactive process
     */
    InteractiveProcess() {
        processType = INTERACTIVE;
        priority = 1;
    }

};

#endif //INTERACTIVE_PROCESS_H
