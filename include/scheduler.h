#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <iostream>
#include <vector>
#include <queue>
#include <deque>
#include <algorithm>
#include <iomanip>
#include <string>
#include <memory>
#include <map>

// Process states
enum ProcessState {
    NEW,
    READY,
    RUNNING,
    WAITING,
    TERMINATED
};

// Process Control Block
class Process {
public:
    int pid;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int priority;
    int waitingTime;
    int turnaroundTime;
    int responseTime;
    int completionTime;
    int startTime;
    ProcessState state;
    bool firstExecution;
    
    Process(int id, int arrival, int burst, int prio = 0);
    void display() const;
};

// Base Scheduler class
class Scheduler {
protected:
    std::vector<Process> processes;
    int currentTime;
    int contextSwitchTime;
    int totalWaitingTime;
    int totalTurnaroundTime;
    int totalResponseTime;
    size_t completedProcesses;
    
public:
    Scheduler(int contextSwitch = 1);
    virtual ~Scheduler() {}
    
    void addProcess(const Process& p);
    virtual void schedule() = 0;
    void displayMetrics();
    void visualizeQueue(const std::deque<Process*>& readyQueue, Process* running);
    void displayProcessStates();
    
    // Getters for testing
    double getAverageWaitingTime() const;
    double getAverageTurnaroundTime() const;
    double getAverageResponseTime() const;
};

// Round Robin Scheduler
class RoundRobinScheduler : public Scheduler {
private:
    int timeQuantum;
    
public:
    RoundRobinScheduler(int quantum, int contextSwitch = 1);
    void schedule() override;
};

// Priority Scheduler (Preemptive)
class PreemptivePriorityScheduler : public Scheduler {
public:
    PreemptivePriorityScheduler(int contextSwitch = 1);
    void schedule() override;
};

// Non-Preemptive Priority Scheduler
class NonPreemptivePriorityScheduler : public Scheduler {
public:
    NonPreemptivePriorityScheduler(int contextSwitch = 1);
    void schedule() override;
};

// Multilevel Queue Scheduler
class MultilevelQueueScheduler : public Scheduler {
private:
    std::deque<Process*> systemQueue;
    std::deque<Process*> interactiveQueue;
    std::deque<Process*> batchQueue;
    int timeQuantum;
    
public:
    MultilevelQueueScheduler(int quantum, int contextSwitch = 1);
    void schedule() override;
};

// Multilevel Feedback Queue Scheduler
class MultilevelFeedbackQueueScheduler : public Scheduler {
private:
    std::deque<Process*> queue0;
    std::deque<Process*> queue1;
    std::deque<Process*> queue2;
    std::map<int, int> processQueue;
    int quantum0 = 8;
    int quantum1 = 16;
    
public:
    MultilevelFeedbackQueueScheduler(int contextSwitch = 1);
    void schedule() override;
};

#endif // SCHEDULER_H