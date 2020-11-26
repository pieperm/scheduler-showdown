#include <iostream>

#include "main.h"
#include "scheduler.h"
#include "process.h"

using namespace std;

int main(int argc, char* argv[]) {

    cout << "Welcome to Scheduler Simulator: Expansion!" << endl << endl;
    ProcessType processType = promptForProcessType();

    int schedulingAlgorithm = promptForSchedulingAlgorithm(processType);
}

ProcessType promptForProcessType() {
    ProcessType processChoice = NONE;

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
            cout << "Invalid process type. You entered " << input << " but 1, 2, or 3 was expected." << endl;
        }

    } while (processChoice == NONE);

    return processChoice;
}

int promptForSchedulingAlgorithm(ProcessType processType) {
    int schedulerChoice = -1;

    do {
        switch (processType) {
            case BATCH:
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
                    cout << "Invalid scheduler selected. You entered " << input << " but 1, 2, 3, or 4 was expected." << endl;
                }

                break;
            case INTERACTIVE:
                cout << "What scheduler would you like to use for interactive processes?" << endl;
                cout << "\t1) Round Robin (RR)" << endl;
                cout << "\t2) Multilevel (ML)" << endl;
                cout << "\t3) Multilevel Feedback (MLF)" << endl;
                cout << "Enter the corresponding number: ";
                cin >> input;

                if(input > 0 && input <= 3) {
                    schedulerChoice = input;
                } else {
                    cout << "Invalid scheduler selected. You entered " << input << " but 1, 2, or 3 was expected." << endl;
                }
                break;
            case REALTIME:
                cout << "What scheduler would you like to use for realtime processes?" << endl;
                cout << "\t1) Rate Monotonic (RM)" << endl;
                cout << "\t2) Earliest Deadline First (EDF)" << endl;
                cout << "Enter the corresponding number: ";
                cin >> input;

                if(input > 0 && input <= 2) {
                    schedulerChoice = input;
                } else {
                    cout << "Invalid scheduler selected. You entered " << input << " but 1 or 2 was expected." << endl;
                }
                break;
            default:
                cout << "Invalid argument processType; expected BATCH, INTERACTIVE, or REALTIME" << endl;
        }
    } while (schedulerChoice == -1);

    return schedulerChoice;

}