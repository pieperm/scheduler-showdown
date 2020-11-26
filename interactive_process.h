#ifndef INTERACTIVE_PROCESS_H
#define INTERACTIVE_PROCESS_H

#include "process.h"

using namespace std;

class InteractiveProcess: public Process {

private:

    int priority;

public:

    InteractiveProcess() {
        processType = INTERACTIVE;
        priority = 1;
    }

};

#endif //INTERACTIVE_PROCESS_H
