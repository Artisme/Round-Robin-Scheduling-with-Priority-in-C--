#include "Process.h"
#include <iostream>
#include <vector>
#include <cstdlib>

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " filename timeq\n";
        return 1;
    }
    int time_quantum{std::atoi(argv[2])};
    std::vector<Process> pVec;
    readProcFile(argv[1], pVec);
    priorityScheduling(pVec);
    prioritySchedulingRR(pVec, time_quantum);
    return 0;
}
