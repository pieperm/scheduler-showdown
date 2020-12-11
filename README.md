# Scheduler Showdown: Expansion

## Information

This project is designed to be an extension to the previous programming assignment Scheduler Showdown. The goal is to expand on the existing project by implementing more types of processes and more scheduling algorithms. 

The original assignment included:
- Round-Robin (RR)
- Shortest Process Next (SPN)
- Shortest Remaining Time (SRT)
- Highest Response Ratio Next (HRRN)

New scheduling algorithms will also include:
- First-In First-Out Scheduling (FIFO)
- Multilevel Scheduling (ML)
- Multilevel Feedback Scheduling (MLF)
- Rate Monotonic Scheduling (RM)
- Earliest Deadline First Scheduling (EDF)

These algorithms will be tested using different types of processes: batch processes, interactive processes, and real-time processes. Sample data will be created to simulate how each algorithm performs depending on the number of processes, duration of processes, time constraints, and other factors.

## Usage

#### Compiling

`cd` into the root directory of the project and use the Makefile:
- `make`: compiles the code into an executable called `program`
- `make clean`: clears up extra files and the executable

#### Running

The `program` executable takes two arguments (in the following order):
1. A relative path to a file containing a list of programs
2. The amount of time to sleep between each clock cycle in the simulation

Running the program with these arguments will then prompt the user to select the
type of process to simulate (batch, interactive, or real-time). After that, they
will be prompted to select the scheduling algorithm for the process type they chose.
The program will then perform a scheduling simulation on the list of processes from the
file using the specified scheduling algorithm.

#### Examples

A number of example process files are already provided in the `processes` directory.
These are organized into `batch`, `interactive`, and `realtime` directories, which contain
processes of corresponding types.

The following examples simulate different process types with a sleep time of 10:

**Batch Process:** `./program.exe processes/batch/all_long.txt 10`

**Interactive Process:** `./program.exe processes/interactive/all_long.txt 10`

**Realtime Process:** `./program.exe processes/realtime/all_long.txt 10`

#### File Format

To make your own process files, each process type follows its own file format:
- Batch: `process_id arrival_time computation_time`
- Interactive: `process_id arrival_time computation_time priority`
- Realtime: `process_id arrival_time computation_time period deadline`
