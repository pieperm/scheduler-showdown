#ifndef PROCESS_H
#define PROCESS_H

#include <string>

using namespace std;

enum ProcessType { NONE: 0, BATCH: 1, INTERACTIVE: 2, REALTIME: 3 };

class Process {

protected:
    string id = -1;
    ProcessType processType;
    bool isDone = false;
    int arrivalTime = -1;

public:

    bool isDone() {
        return isDone;
    }

};

#endif //PROCESS_H
