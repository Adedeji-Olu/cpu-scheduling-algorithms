# API Documentation

## Table of Contents
1. [Process Class](#process-class)
2. [Scheduler Base Class](#scheduler-base-class)
3. [Round Robin Scheduler](#round-robin-scheduler)
4. [Priority Schedulers](#priority-schedulers)
5. [Multilevel Schedulers](#multilevel-schedulers)

---

## Process Class

Represents a Process Control Block (PCB).

### Constructor
```cpp
Process(int pid, int arrivalTime, int burstTime, int priority = 0)
```

**Parameters:**
- `pid`: Process identifier (unique)
- `arrivalTime`: Time when process enters system
- `burstTime`: Total CPU time required
- `priority`: Process priority (0 = highest, 5 = lowest)

### Public Members
- `int pid` - Process ID
- `int arrivalTime` - Arrival time in system
- `int burstTime` - Total CPU burst time
- `int remainingTime` - Remaining CPU time
- `int priority` - Scheduling priority
- `int waitingTime` - Time spent waiting
- `int turnaroundTime` - Total time in system
- `int responseTime` - Time until first execution
- `int completionTime` - Time when completed
- `ProcessState state` - Current process state

### Methods
```cpp
void display() const
```
Prints process information to console.

---

## Scheduler Base Class

Abstract base class for all scheduling algorithms.

### Constructor
```cpp
Scheduler(int contextSwitchTime = 1)
```

**Parameters:**
- `contextSwitchTime`: Overhead for context switching (default: 1)

### Public Methods

#### addProcess
```cpp
void addProcess(const Process& p)
```
Adds a process to the scheduler.

#### schedule
```cpp
virtual void schedule() = 0
```
Pure virtual method - executes the scheduling algorithm.

#### displayMetrics
```cpp
void displayMetrics()
```
Displays performance metrics for all processes.

#### displayProcessStates
```cpp
void displayProcessStates()
```
Shows current state of all processes in table format.

#### Metric Getters
```cpp
double getAverageWaitingTime() const
double getAverageTurnaroundTime() const
double getAverageResponseTime() const
```

---

## Round Robin Scheduler

Implements time-sharing with configurable quantum.

### Constructor
```cpp
RoundRobinScheduler(int timeQuantum, int contextSwitchTime = 1)
```

**Parameters:**
- `timeQuantum`: Time slice for each process
- `contextSwitchTime`: Context switch overhead

### Usage Example
```cpp
RoundRobinScheduler rr(4, 1);  // Quantum = 4, overhead = 1
rr.addProcess(Process(1, 0, 10, 0));
rr.addProcess(Process(2, 1, 5, 0));
rr.schedule();
```

---

## Priority Schedulers

### Preemptive Priority
Higher priority processes can preempt lower priority ones.
```cpp
PreemptivePriorityScheduler(int contextSwitchTime = 1)
```

### Non-Preemptive Priority
Processes run to completion once started.
```cpp
NonPreemptivePriorityScheduler(int contextSwitchTime = 1)
```

### Priority Values
- 0 = Highest priority
- 5 = Lowest priority

---

## Multilevel Schedulers

### Multilevel Queue
Three fixed-priority queues.
```cpp
MultilevelQueueScheduler(int timeQuantum, int contextSwitchTime = 1)
```

**Queue Structure:**
- System Queue: Priority 0-1 (highest)
- Interactive Queue: Priority 2-3
- Batch Queue: Priority 4-5 (lowest)

### Multilevel Feedback Queue
Adaptive scheduling with queue demotion.
```cpp
MultilevelFeedbackQueueScheduler(int contextSwitchTime = 1)
```

**Queue Configuration:**
- Q0: Time quantum = 8 (highest priority)
- Q1: Time quantum = 16
- Q2: FCFS (lowest priority)

---

## Performance Metrics

### Waiting Time
Time process spends in ready queue.
```
Waiting Time = Turnaround Time - Burst Time
```

### Turnaround Time
Total time from arrival to completion.
```
Turnaround Time = Completion Time - Arrival Time
```

### Response Time
Time from arrival to first CPU access.
```
Response Time = First Start Time - Arrival Time
```

---

## Process States
```cpp
enum ProcessState {
    NEW,        // Process created
    READY,      // In ready queue
    RUNNING,    // Executing on CPU
    WAITING,    // Blocked (I/O) - not used in current implementation
    TERMINATED  // Completed
}
```

---

## Error Handling

All schedulers handle:
- Empty process lists
- Single process execution
- Simultaneous arrivals
- Zero burst times (prevented by design)

---

## Thread Safety

**Note:** Current implementation is **not thread-safe**. Use in single-threaded environments only.

---

## Version

Current API Version: 1.0.0
Last Updated: December 2024
