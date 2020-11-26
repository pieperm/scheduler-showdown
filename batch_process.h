#ifndef BATCH_PROCESS_H
#define BATCH_PROCESS_H

#include "process.h"

class BatchProcess: public Process {

public:

    BatchProcess() {
        processType = BATCH;
    }

};

#endif //BATCH_PROCESS_H
