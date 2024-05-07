# RoundRobinPriority
A Round Robin Scheduling with Priority in C++  
```
g++ main.cpp process.cpp
```
In the data file, each line contains four parameters <id, a, b, p>, where id is the process
identifier, a is the arrival time, b is the burst time, and p is the priority. The process identifier has at most 3 characters.
Arrival times range from 0 to 20 ms. Burst times range from 0 to 40 ms. Priority levels range from 1 to 10. Low
numbers represent high priority. If there are processes having the same priority, the scheduling order follows the
FCFS policy according to the order in the file

p1 0 4 3  
p2 0 5 2  
p3 0 8 2  
p4 0 7 1  
p5 0 3 3  

Suppose the filename is data.dat. Here is a possible execution scenario (2 ms is the time quantum):
```
prog data.dat 2
```
