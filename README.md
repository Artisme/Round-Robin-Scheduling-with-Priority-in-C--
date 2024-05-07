# RoundRobinPriority
A Round Robin Scheduling with Priority in C++  
The program first reads the file containing processes into a vector. Then it sorts the vector based on the
arrival time. Processes with same arrival time will be ordered based on the order it was read.  
This program uses a user-defined class called Process and ProcessQueue. The Process object contains the
arrival time, id, burst time, priority, and the remaining time while running. The ProcessQueue inherits
std::priority_queue to implement it. It modifies the original push() and top() methods. This queue has
one extra attribute called “entry”. This attribute functions as a order counter for processes to correct the
order of processes with same priority. The function object used by the priority queue is defined as
PQCompare. It compares the priority and the entry order of processes with same priority.  
Both of these schedulers run on 2 phases, when the vector of processes read directly from file haven’t
emptied, and when the ProcessQueue still contains processes. The result of the scheduling is kept in a
vector of start time, end time, and process id. It is outputted at the end of the scheduling.  

## Priority Scheduling
while(!processes.empty()) phase
It considers 4 cases,
1. Processes with same arrival time && currentProcess haven’t run yet
In this case, the program will consume all the processes with same arrival time, push it into the
ProcessQueue. It will also keep track of the highest priority process. This process will be the
current running process after the loop ends.
2. Current running process has reached the end of running time and the current time is the same
the next process arrival time
In this case, it will compare the next process in the vector and the next process in the
ProcessQueue. If these processes have the same priority, it will prioritize the PriorityQueue as
the next running process.
3. The current time is the same the next process (vector) arrival time
It will compare their priority. If they have the same priority, the current process will keep
running.
4. The current process has reached the end time
If the ProcessQueue is not empty, it will assign the top of the queue as the next running process.
Otherwise, it will jump the next process in vector.
while(!pq.empty())
It loops the ProcessQueue and push the result into the result vector
## Round Robin Scheduling with Priority
This scheduling in the first phase has the same code as the first scheduler with slight differences. The
end time in each cases is checked if it doesn’t exceed the time quantum.
In the second phase, it is also quite similar to the first scheduler with some changes in the end time
implementation.
## Compiling and Running
In the same working directory,
```
g++ Process.cpp main.cpp -o a.out
```
To run the program
```
a.out <data.dat> <time quantum>  
```
In the data file, each line contains four parameters <id, a, b, p>, where id is the process
identifier, a is the arrival time, b is the burst time, and p is the priority. The process identifier has at most 3 characters.
Arrival times range from 0 to 20 ms. Burst times range from 0 to 40 ms. Priority levels range from 1 to 10. Low
numbers represent high priority. If there are processes having the same priority, the scheduling order follows the
FCFS policy according to the order in the file
```
p1 0 4 3  
p2 0 5 2  
p3 0 8 2  
p4 0 7 1  
p5 0 3 3
```
