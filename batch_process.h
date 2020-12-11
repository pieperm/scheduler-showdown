#ifndef BATCH_PROCESS_H
#define BATCH_PROCESS_H

#include "process.h"

/**
 * Class representing a batch process
 * Child class of Process
 */
class BatchProcess: public Process {

public:

    /**
     * Constructor for a batch process
     */
    BatchProcess() {
        processType = BATCH;
    }

};

#endif //BATCH_PROCESS_H
