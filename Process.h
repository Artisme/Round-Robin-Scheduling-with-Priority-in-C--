#pragma once

#include <queue>
#include <string>
#include <vector>

class Process {
   public:
    Process(std::string pid, int arrivalTime, int burstTime, int priority);

    std::string pid;
    int arrivalTime{0};
    int burstTime{0};
    int priority{0};
    int remainingTime{0};
};

class PQCompare {
   public:
    bool operator()(const std::pair<int, Process> &p1,
                    const std::pair<int, Process> &p2);
};

class ProcessQueue
    : public std::priority_queue<std::pair<int, Process>,
                                 std::vector<std::pair<int, Process>>,
                                 PQCompare> {
   public:
    ProcessQueue();
    void push(const Process &p);
    const Process &top() const;

   private:
    int entry{0};
};

// Print out the priority scheduling Gantt chart of processes vector
void priorityScheduling(std::vector<Process> processes);
void prioritySchedulingRR(std::vector<Process> processes, const int &tq);
// Read processes from file and sort by arrival time
void readProcFile(char *filename, std::vector<Process> &v);
void printProcess(ProcessQueue pq);
