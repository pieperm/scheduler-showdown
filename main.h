#ifndef MAIN_H
#define MAIN_H

#include "process.h"

void readProcessFile(const string& filename, const ProcessType processType, vector<Process*>& procList);
ProcessType promptForProcessType();
int promptForSchedulingAlgorithm(ProcessType processType);
void outputStatistics(vector<Process*>& processList, ProcessType selectedProcessType, int timesIdle, int currentTime);

#endif //MAIN_H
