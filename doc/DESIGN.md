# Design Document

## 1. System Architecture

### 1.1 Overview
The CPU Scheduler Simulator implements five classic scheduling algorithms using object-oriented design principles in C++17.

### 1.2 Class Hierarchy
```
Scheduler (Abstract Base Class)
    ├── RoundRobinScheduler
    ├── PreemptivePriorityScheduler
    ├── NonPreemptivePriorityScheduler
    ├── MultilevelQueueScheduler
    └── MultilevelFeedbackQueueScheduler
```

### 1.3 Design Patterns

#### Template Method Pattern
The base `Scheduler` class defines the skeleton:
- Common metrics calculation
- Process management
- Visualization framework

Derived classes implement specific `schedule()` logic.

#### Strategy Pattern
Different scheduling algorithms are interchangeable strategies.

---

## 2. Data Structures

### 2.1 Process Management
```cpp
std::vector<Process> processes  // All processes
std::deque<Process*> readyQueue  // FIFO queue for RR
std::priority_queue<Process*> priorityQueue  // Min-heap for priority
```

**Rationale:**
- `vector`: O(1) random access, cache-friendly
- `deque`: O(1) front/back operations for FIFO
- `priority_queue`: O(log n) insertions, O(1) top access

### 2.2 Time Complexity Analysis

| Algorithm | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| Round Robin | O(n) | O(n) |
| Preemptive Priority | O(n log n) | O(n) |
| Non-Preemptive Priority | O(n log n) | O(n) |
| Multilevel Queue | O(n) | O(n) |
| MLFQ | O(n log n) | O(n) |

Where n = number of processes

---

## 3. Algorithm Implementations

### 3.1 Round Robin
**Mechanism:**
- Circular ready queue
- Fixed time quantum
- FIFO process selection

**Pseudocode:**
```
while processes remain:
    if new processes arrived:
        add to ready queue
    if current process finished:
        calculate metrics
        context switch
    else if quantum expired:
        move to back of queue
        context switch
    select next from front of queue
    execute for 1 time unit
    increment time
```

### 3.2 Priority Scheduling

**Preemptive:**
- Uses min-heap (priority queue)
- Checks for higher priority on each arrival
- Preempts if necessary

**Non-Preemptive:**
- Same data structure
- No preemption once running
- Simpler implementation

### 3.3 Multilevel Queue
**Queue Structure:**
```
System (0-1) → Always scheduled first
    ↓
Interactive (2-3) → Round Robin, quantum=4
    ↓
Batch (4-5) → Round Robin or FCFS
```

**Scheduling:** Strict priority between queues

### 3.4 Multilevel Feedback Queue
**Adaptive Mechanism:**
```
New process → Q0 (quantum=8)
    ↓ (quantum expires)
    Q1 (quantum=16)
    ↓ (quantum expires)
    Q2 (FCFS)
```

**Aging:** Prevents starvation by prioritizing Q0

---

## 4. Context Switching Model

### 4.1 Implementation
```cpp
if (context_switch_needed) {
    currentTime += contextSwitchTime;
}
```

### 4.2 When Context Switch Occurs
1. Process completion
2. Quantum expiry (RR, MLQ)
3. Preemption (Preemptive Priority)
4. Queue demotion (MLFQ)

---

## 5. Performance Metrics

### 5.1 Calculation Formulas

**Waiting Time:**
```cpp
waitingTime = turnaroundTime - burstTime
```

**Turnaround Time:**
```cpp
turnaroundTime = completionTime - arrivalTime
```

**Response Time:**
```cpp
responseTime = firstStartTime - arrivalTime
```

### 5.2 Aggregation
```cpp
avgMetric = totalMetric / numberOfProcesses
```

---

## 6. Design Decisions

### 6.1 Why Object-Oriented?
- **Reusability:** Common functionality in base class
- **Extensibility:** Easy to add new algorithms
- **Maintainability:** Clear separation of concerns

### 6.2 Why C++ STL?
- **Performance:** Highly optimized containers
- **Reliability:** Well-tested implementations
- **Portability:** Standard across platforms

### 6.3 Why No I/O Simulation?
- **Scope:** Focus on CPU scheduling
- **Complexity:** I/O adds significant complexity
- **Educational:** CPU-bound clearer for learning

---

## 7. System Limitations

### 7.1 Current Limitations
1. **No I/O modeling** - CPU-bound only
2. **Discrete time** - Integer time units
3. **No multiprocessing** - Single CPU
4. **No real-time constraints** - No deadlines
5. **No dynamic priority** - Except MLFQ

### 7.2 Assumptions
- All processes are CPU-bound
- Burst times known in advance
- No process creation during execution
- No inter-process communication

---

## 8. Future Enhancements

### 8.1 Planned Features
1. **I/O Burst Modeling**
   - Add WAITING state
   - Device queues
   - I/O completion events

2. **Real-Time Scheduling**
   - Earliest Deadline First (EDF)
   - Rate Monotonic Scheduling (RMS)
   - Hard/soft deadline enforcement

3. **Multiprocessor Support**
   - Load balancing
   - Processor affinity
   - Gang scheduling

4. **Advanced Metrics**
   - CPU utilization
   - Throughput
   - Context switch overhead analysis

---

## 9. Testing Strategy

### 9.1 Unit Tests
- Process creation/initialization
- Queue operations
- Metric calculations
- Edge cases

### 9.2 System Tests
- End-to-end algorithm execution
- Multi-algorithm comparison
- Large process sets
- Stress testing

### 9.3 Benchmarks
- Execution time measurement
- Scalability analysis
- Algorithm comparison

---

## 10. References

1. Silberschatz, A., Galvin, P. B., & Gagne, G. (2018). *Operating System Concepts* (10th ed.). Wiley.
2. Tanenbaum, A. S., & Bos, H. (2014). *Modern Operating Systems* (4th ed.). Pearson.
3. Stallings, W. (2018). *Operating Systems: Internals and Design Principles* (9th ed.). Pearson.

---

**Document Version:** 1.0  
**Last Updated:** December 2024  
**Authors:** [Team Members]
