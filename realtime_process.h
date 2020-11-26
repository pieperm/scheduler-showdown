#ifndef REALTIME_PROCESS_H
#define REALTIME_PROCESS_H

#include "process.h"

class RealtimeProcess: public Process {

public:
    RealtimeProcess() {
        processType = REALTIME;
    }

};

#endif //REALTIME_PROCESS_H
