#include "Process.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <utility>
#include <vector>

Process::Process(std::string pid, int arrivalTime, int burstTime, int priority)
    : pid{pid},
      arrivalTime{arrivalTime},
      burstTime{burstTime},
      priority{priority},
      remainingTime{burstTime} {};

ProcessQueue::ProcessQueue()
    : std::priority_queue<std::pair<int, Process>,
                          std::vector<std::pair<int, Process>>, PQCompare>() {}

bool PQCompare::operator()(const std::pair<int, Process> &p1,
                           const std::pair<int, Process> &p2) {
    if (p1.second.priority == p2.second.priority) {
        return p1.first > p2.first;
    }
    return p1.second.priority > p2.second.priority;
}

void ProcessQueue::push(const Process &p) {
    std::priority_queue<std::pair<int, Process>,
                        std::vector<std::pair<int, Process>>,
                        PQCompare>::push({entry, p});
    ++entry;
}

const Process &ProcessQueue::top() const {
    return std::priority_queue<std::pair<int, Process>,
                               std::vector<std::pair<int, Process>>,
                               PQCompare>::top()
        .second;
}

void priorityScheduling(std::vector<Process> processes) {
    ProcessQueue pq{};
    std::vector<std::pair<std::pair<int, int>, std::string>> schedulingResult;
    Process currentProcess{processes.front()};
    processes.erase(processes.begin());
    int currentTime{currentProcess.arrivalTime}, beginTime{currentTime},
        startTime{currentTime}, endTime{startTime + currentProcess.burstTime};

    while (!processes.empty()) {
        if (currentTime == processes.front().arrivalTime &&
            currentTime == startTime) {
            std::vector<Process> tmpV{currentProcess};
            while (!processes.empty() &&
                   currentTime == processes.front().arrivalTime) {
                if (currentProcess.priority > processes.front().priority) {
                    currentProcess = processes.front();
                }
                tmpV.push_back(processes.front());
                processes.erase(processes.begin());
            }
            for (const auto &x : tmpV) {
                if (x.pid != currentProcess.pid) {
                    pq.push(x);
                }
            }
            endTime = currentTime + currentProcess.remainingTime;
        } else if (currentTime == processes.front().arrivalTime &&
                   currentTime == endTime) {
            schedulingResult.push_back(
                {{startTime, currentTime}, currentProcess.pid});
            currentProcess = processes.front();
            if (!pq.empty() && (currentProcess.priority >= pq.top().priority)) {
                currentProcess = pq.top();
                pq.pop();
                pq.push(processes.front());
            }
            processes.erase(processes.begin());
            startTime = currentTime;
            endTime = currentTime + currentProcess.remainingTime;
        } else if (currentTime == processes.front().arrivalTime) {
            if (currentProcess.priority <= processes.front().priority) {
                pq.push(processes.front());
                processes.erase(processes.begin());
                continue;
            }

            schedulingResult.push_back(
                {{startTime, currentTime}, currentProcess.pid});
            currentProcess.remainingTime -= currentTime - startTime;
            pq.push(currentProcess);
            currentProcess = processes.front();
            processes.erase(processes.begin());
            startTime = currentTime;
            endTime = currentTime + currentProcess.remainingTime;
        } else if (currentTime == endTime) {
            schedulingResult.push_back(
                {{startTime, currentTime}, currentProcess.pid});
            if (!pq.empty()) {
                currentProcess = pq.top();
                pq.pop();
                startTime = currentTime;
            } else {
                currentProcess = processes.front();
                processes.erase(processes.begin());
                startTime = currentProcess.arrivalTime;
                currentTime = startTime;
            }
            endTime = startTime + currentProcess.remainingTime;
        } else {
            ++currentTime;
        }
    }

    while (!pq.empty()) {
        currentTime = endTime;
        schedulingResult.push_back({{startTime, endTime}, currentProcess.pid});
        startTime = currentTime;
        currentProcess = pq.top();
        pq.pop();
        endTime = currentTime + currentProcess.remainingTime;
    }
    schedulingResult.push_back({{startTime, endTime}, currentProcess.pid});

    std::cout << "Priority scheduling" << '\n';
    std::string &pid{schedulingResult.front().second};
    startTime = endTime = beginTime;
    for (auto it = schedulingResult.begin(); it != schedulingResult.end();
         ++it) {
        if (pid == it->second) {
            endTime = it->first.second;
            continue;
        }
        if (startTime != endTime) {
            std::cout << startTime << ',' << endTime << ' ' << pid << '\n';
        }
        startTime = it->first.first;
        endTime = it->first.second;
        pid = it->second;
    }
    std::cout << startTime << ',' << endTime << ' ' << pid << '\n';
}

void prioritySchedulingRR(std::vector<Process> processes, const int &tq) {
    ProcessQueue pq{};
    std::vector<std::pair<std::pair<int, int>, std::string>> schedulingResult;
    Process currentProcess{processes.front()};
    processes.erase(processes.begin());
    int currentTime{currentProcess.arrivalTime}, beginTime{currentTime},
        startTime{currentTime}, endTime{0};
    if (currentProcess.remainingTime > tq) {
        endTime = currentTime + tq;
    } else {
        endTime = currentTime + currentProcess.remainingTime;
    }

    while (!processes.empty()) {
        if (currentTime == processes.front().arrivalTime &&
            currentTime == startTime) {
            std::vector<Process> tmpV{currentProcess};
            while (!processes.empty() &&
                   currentTime == processes.front().arrivalTime) {
                if (currentProcess.priority > processes.front().priority) {
                    currentProcess = processes.front();
                }
                tmpV.push_back(processes.front());
                processes.erase(processes.begin());
            }
            for (const auto &x : tmpV) {
                if (x.pid != currentProcess.pid) {
                    pq.push(x);
                }
            }
            if (currentProcess.remainingTime > tq) {
                endTime = currentTime + tq;
            } else {
                endTime = currentTime + currentProcess.remainingTime;
            }
        } else if (currentTime == processes.front().arrivalTime &&
                   currentTime == endTime) {
            schedulingResult.push_back(
                {{startTime, currentTime}, currentProcess.pid});
            currentProcess.remainingTime -= currentTime - startTime;
            if (currentProcess.remainingTime > 0) {
                pq.push(currentProcess);
            }
            currentProcess = processes.front();
            if (!pq.empty() && (currentProcess.priority >= pq.top().priority)) {
                currentProcess = pq.top();
                pq.pop();
                pq.push(processes.front());
            }
            processes.erase(processes.begin());
            startTime = currentTime;
            if (currentProcess.remainingTime > tq) {
                endTime = currentTime + tq;
            } else {
                endTime = currentTime + currentProcess.remainingTime;
            }
        } else if (currentTime == processes.front().arrivalTime) {
            if (currentProcess.priority <= processes.front().priority) {
                pq.push(processes.front());
                processes.erase(processes.begin());
                continue;
            }

            schedulingResult.push_back(
                {{startTime, currentTime}, currentProcess.pid});
            currentProcess.remainingTime -= currentTime - startTime;
            pq.push(currentProcess);
            currentProcess = processes.front();
            processes.erase(processes.begin());
            startTime = currentTime;
            if (currentProcess.remainingTime > tq) {
                endTime = currentTime + tq;
            } else {
                endTime = currentTime + currentProcess.remainingTime;
            }
        } else if (currentTime == endTime) {
            schedulingResult.push_back(
                {{startTime, currentTime}, currentProcess.pid});
            currentProcess.remainingTime -= currentTime - startTime;

            if (currentProcess.remainingTime > 0) {
                pq.push(currentProcess);
            }
            if (!pq.empty()) {
                currentProcess = pq.top();
                pq.pop();
                startTime = currentTime;
            } else {
                currentProcess = processes.front();
                processes.erase(processes.begin());
                startTime = currentProcess.arrivalTime;
                currentTime = startTime;
            }
            if (currentProcess.remainingTime > tq) {
                endTime = currentTime + tq;
            } else {
                endTime = currentTime + currentProcess.remainingTime;
            }
        } else {
            ++currentTime;
        }
    }

    while (!pq.empty()) {
        currentTime = endTime;
        schedulingResult.push_back({{startTime, endTime}, currentProcess.pid});
        currentProcess.remainingTime -= currentTime - startTime;
        if (currentProcess.remainingTime > 0) {
            pq.push(currentProcess);
        }
        currentProcess = pq.top();
        pq.pop();
        startTime = currentTime;
        if (currentProcess.remainingTime > tq) {
            endTime = currentTime + tq;
        } else {
            endTime = currentTime + currentProcess.remainingTime;
        }
    }
    schedulingResult.push_back(
        {{startTime, startTime + currentProcess.remainingTime},
         currentProcess.pid});

    std::cout << "Priority scheduling RR:" << '\n';
    std::string &pid{schedulingResult.front().second};
    startTime = endTime = beginTime;
    for (auto it = schedulingResult.begin(); it != schedulingResult.end();
         ++it) {
        if (pid == it->second) {
            endTime = it->first.second;
            continue;
        }
        if (startTime != endTime) {
            std::cout << startTime << ',' << endTime << ' ' << pid << '\n';
        }
        startTime = it->first.first;
        endTime = it->first.second;
        pid = it->second;
    }
    std::cout << startTime << ',' << endTime << ' ' << pid << '\n';
}

void printProcess(ProcessQueue pq) {
    std::cout << "---\n";
    while (!pq.empty()) {
        Process proc = pq.top();
        pq.pop();
        std::cout << proc.pid << '\n';
    }
}

void readProcFile(char *filename, std::vector<Process> &v) {
    std::fstream fs(filename, std::fstream::in);
    std::string tmpStr;
    while (std::getline(fs, tmpStr)) {
        std::stringstream ss(tmpStr);
        std::string pid;
        int arrivalTime, burstTime, priority;
        ss >> pid >> arrivalTime >> burstTime >> priority;
        v.push_back({pid, arrivalTime, burstTime, priority});
    }
    fs.close();
    std::sort(v.begin(), v.end(), [](const Process &p1, const Process &p2) {
        return p1.arrivalTime < p2.arrivalTime;
    });
}
