// src/scheduler.cpp
// Implementation of all scheduler classes

#include "scheduler.h"

using namespace std;

// ============== Process Implementation ==============
Process::Process(int id, int arrival, int burst, int prio)
    : pid(id), arrivalTime(arrival), burstTime(burst), 
      remainingTime(burst), priority(prio), waitingTime(0),
      turnaroundTime(0), responseTime(-1), completionTime(0),
      startTime(-1), state(NEW), firstExecution(true) {}

void Process::display() const {
    cout << "P" << pid << " [Arrival: " << arrivalTime 
         << ", Burst: " << burstTime << ", Priority: " << priority 
         << ", Remaining: " << remainingTime << "]";
}

// ============== Base Scheduler Implementation ==============
Scheduler::Scheduler(int contextSwitch)
    : currentTime(0), contextSwitchTime(contextSwitch),
      totalWaitingTime(0), totalTurnaroundTime(0),
      totalResponseTime(0), completedProcesses(0U) {}

void Scheduler::addProcess(const Process& p) {
    processes.push_back(p);
}

void Scheduler::displayMetrics() {
    cout << "\n========== Performance Metrics ==========\n";
    cout << fixed << setprecision(2);
    
    for (const auto& p : processes) {
        cout << "\nProcess P" << p.pid << ":\n";
        cout << "  Waiting Time: " << p.waitingTime << " ms\n";
        cout << "  Turnaround Time: " << p.turnaroundTime << " ms\n";
        cout << "  Response Time: " << p.responseTime << " ms\n";
        cout << "  Completion Time: " << p.completionTime << " ms\n";
    }
    
    double avgWaiting = (double)totalWaitingTime / processes.size();
    double avgTurnaround = (double)totalTurnaroundTime / processes.size();
    double avgResponse = (double)totalResponseTime / processes.size();
    
    cout << "\n========== Average Metrics ==========\n";
    cout << "Average Waiting Time: " << avgWaiting << " ms\n";
    cout << "Average Turnaround Time: " << avgTurnaround << " ms\n";
    cout << "Average Response Time: " << avgResponse << " ms\n";
    cout << "=========================================\n";
}

void Scheduler::visualizeQueue(const deque<Process*>& readyQueue, Process* running) {
    cout << "\n[Time " << currentTime << "]";
    cout << " CPU: ";
    if (running) {
        cout << "P" << running->pid << "(rem:" << running->remainingTime << ")";
    } else {
        cout << "IDLE";
    }
    
    cout << " | Ready Queue: [";
    for (size_t i = 0; i < readyQueue.size(); i++) {
        cout << "P" << readyQueue[i]->pid << "(rem:" << readyQueue[i]->remainingTime << ")";
        if (i < readyQueue.size() - 1) cout << ", ";
    }
    cout << "]";
    
    int utilization = 0;
    for (const auto& p : processes) {
        if (p.state == RUNNING || p.state == TERMINATED) {
            utilization++;
        }
    }
    cout << " | Utilization: " << (utilization * 100 / processes.size()) << "%\n";
}

void Scheduler::displayProcessStates() {
    cout << "\n+-----+-------+--------+-----------+-------+\n";
    cout << "| PID | State | Burst  | Remaining | Queue |\n";
    cout << "+-----+-------+--------+-----------+-------+\n";
    
    for (const auto& p : processes) {
        cout << "| P" << p.pid << "  | ";
        
        switch(p.state) {
            case NEW: cout << "NEW  "; break;
            case READY: cout << "READY"; break;
            case RUNNING: cout << "RUN  "; break;
            case WAITING: cout << "WAIT "; break;
            case TERMINATED: cout << "DONE "; break;
        }
        
        cout << " |   " << setw(2) << p.burstTime << "    |    "
             << setw(2) << p.remainingTime << "      | ";
        
        if (p.state == READY) cout << "Yes";
        else if (p.state == RUNNING) cout << "CPU";
        else cout << "No ";
        
        cout << "  |\n";
    }
    cout << "+-----+-------+--------+-----------+-------+\n";
}

double Scheduler::getAverageWaitingTime() const {
    return (double)totalWaitingTime / processes.size();
}

double Scheduler::getAverageTurnaroundTime() const {
    return (double)totalTurnaroundTime / processes.size();
}

double Scheduler::getAverageResponseTime() const {
    return (double)totalResponseTime / processes.size();
}

// ============== Round Robin Implementation ==============
RoundRobinScheduler::RoundRobinScheduler(int quantum, int contextSwitch)
    : Scheduler(contextSwitch), timeQuantum(quantum) {}

void RoundRobinScheduler::schedule() {
    cout << "\n========== Round Robin Scheduling (Quantum=" << timeQuantum << ") ==========\n";
    deque<Process*> readyQueue;
    Process* currentProcess = nullptr;
    int timeSlice = 0;
    
    sort(processes.begin(), processes.end(), 
         [](const Process& a, const Process& b) { return a.arrivalTime < b.arrivalTime; });
    
    size_t nextArrival = 0;
    
    while (completedProcesses < processes.size()) {
        // Add newly arrived processes
        while (nextArrival < processes.size() && 
               processes[nextArrival].arrivalTime <= currentTime) {
            processes[nextArrival].state = READY;
            readyQueue.push_back(&processes[nextArrival]);
            nextArrival++;
        }
        
        // Context switch if needed
        if (currentProcess != nullptr) {
            if (currentProcess->remainingTime == 0) {
                currentProcess->state = TERMINATED;
                currentProcess->completionTime = currentTime;
                currentProcess->turnaroundTime = currentTime - currentProcess->arrivalTime;
                currentProcess->waitingTime = currentProcess->turnaroundTime - currentProcess->burstTime;
                
                totalWaitingTime += currentProcess->waitingTime;
                totalTurnaroundTime += currentProcess->turnaroundTime;
                totalResponseTime += currentProcess->responseTime;
                
                completedProcesses++;
                currentProcess = nullptr;
                timeSlice = 0;
                
                if (!readyQueue.empty()) {
                    currentTime += contextSwitchTime;
                }
            } else if (timeSlice >= timeQuantum) {
                currentProcess->state = READY;
                readyQueue.push_back(currentProcess);
                currentProcess = nullptr;
                timeSlice = 0;
                currentTime += contextSwitchTime;
            }
        }
        
        // Select next process
        if (currentProcess == nullptr && !readyQueue.empty()) {
            currentProcess = readyQueue.front();
            readyQueue.pop_front();
            currentProcess->state = RUNNING;
            
            if (currentProcess->firstExecution) {
                currentProcess->responseTime = currentTime - currentProcess->arrivalTime;
                currentProcess->startTime = currentTime;
                currentProcess->firstExecution = false;
            }
            timeSlice = 0;
        }
        
        visualizeQueue(readyQueue, currentProcess);
        
        // Display detailed state every 5 time units
        if (currentTime % 5 == 0) {
            displayProcessStates();
        }
        
        if (currentProcess != nullptr) {
            currentProcess->remainingTime--;
            timeSlice++;
        }
        
        currentTime++;
        
        if (currentProcess == nullptr && readyQueue.empty() && nextArrival < processes.size()) {
            currentTime = processes[nextArrival].arrivalTime;
        }
    }
    
    displayMetrics();
}

// ============== Preemptive Priority Implementation ==============
PreemptivePriorityScheduler::PreemptivePriorityScheduler(int contextSwitch)
    : Scheduler(contextSwitch) {}

void PreemptivePriorityScheduler::schedule() {
    cout << "\n========== Preemptive Priority Scheduling (Lower number = Higher priority) ==========\n";
    
    auto comparePriority = [](Process* a, Process* b) {
        if (a->priority == b->priority) {
            return a->arrivalTime > b->arrivalTime;
        }
        return a->priority > b->priority;
    };
    
    priority_queue<Process*, vector<Process*>, decltype(comparePriority)> readyQueue(comparePriority);
    Process* currentProcess = nullptr;
    
    sort(processes.begin(), processes.end(), 
         [](const Process& a, const Process& b) { return a.arrivalTime < b.arrivalTime; });
    
    size_t nextArrival = 0;
    
    while (completedProcesses < processes.size()) {
        while (nextArrival < processes.size() && 
               processes[nextArrival].arrivalTime <= currentTime) {
            processes[nextArrival].state = READY;
            readyQueue.push(&processes[nextArrival]);
            
            if (currentProcess != nullptr && 
                processes[nextArrival].priority < currentProcess->priority) {
                currentProcess->state = READY;
                readyQueue.push(currentProcess);
                currentProcess = nullptr;
                currentTime += contextSwitchTime;
            }
            nextArrival++;
        }
        
        if (currentProcess != nullptr && currentProcess->remainingTime == 0) {
            currentProcess->state = TERMINATED;
            currentProcess->completionTime = currentTime;
            currentProcess->turnaroundTime = currentTime - currentProcess->arrivalTime;
            currentProcess->waitingTime = currentProcess->turnaroundTime - currentProcess->burstTime;
            
            totalWaitingTime += currentProcess->waitingTime;
            totalTurnaroundTime += currentProcess->turnaroundTime;
            totalResponseTime += currentProcess->responseTime;
            
            completedProcesses++;
            currentProcess = nullptr;
            
            if (!readyQueue.empty()) {
                currentTime += contextSwitchTime;
            }
        }
        
        if (currentProcess == nullptr && !readyQueue.empty()) {
            currentProcess = const_cast<Process*>(readyQueue.top());
            readyQueue.pop();
            currentProcess->state = RUNNING;
            
            if (currentProcess->firstExecution) {
                currentProcess->responseTime = currentTime - currentProcess->arrivalTime;
                currentProcess->startTime = currentTime;
                currentProcess->firstExecution = false;
            }
        }
        
        deque<Process*> queueCopy;
        auto tempQueue = readyQueue;
        while (!tempQueue.empty()) {
            queueCopy.push_back(const_cast<Process*>(tempQueue.top()));
            tempQueue.pop();
        }
        visualizeQueue(queueCopy, currentProcess);
        
        // Display detailed state every 5 time units
        if (currentTime % 5 == 0) {
            displayProcessStates();
        }
        
        if (currentProcess != nullptr) {
            currentProcess->remainingTime--;
        }
        
        currentTime++;
        
        if (currentProcess == nullptr && readyQueue.empty() && nextArrival < processes.size()) {
            currentTime = processes[nextArrival].arrivalTime;
        }
    }
    
    displayMetrics();
}

// ============== Non-Preemptive Priority Implementation ==============
NonPreemptivePriorityScheduler::NonPreemptivePriorityScheduler(int contextSwitch)
    : Scheduler(contextSwitch) {}

void NonPreemptivePriorityScheduler::schedule() {
    cout << "\n========== Non-Preemptive Priority Scheduling (Lower number = Higher priority) ==========\n";
    
    auto comparePriority = [](Process* a, Process* b) {
        if (a->priority == b->priority) {
            return a->arrivalTime > b->arrivalTime;
        }
        return a->priority > b->priority;
    };
    
    priority_queue<Process*, vector<Process*>, decltype(comparePriority)> readyQueue(comparePriority);
    Process* currentProcess = nullptr;
    
    sort(processes.begin(), processes.end(), 
         [](const Process& a, const Process& b) { return a.arrivalTime < b.arrivalTime; });
    
    size_t nextArrival = 0;
    
    while (completedProcesses < processes.size()) {
        while (nextArrival < processes.size() && 
               processes[nextArrival].arrivalTime <= currentTime) {
            processes[nextArrival].state = READY;
            readyQueue.push(&processes[nextArrival]);
            nextArrival++;
        }
        
        if (currentProcess != nullptr && currentProcess->remainingTime == 0) {
            currentProcess->state = TERMINATED;
            currentProcess->completionTime = currentTime;
            currentProcess->turnaroundTime = currentTime - currentProcess->arrivalTime;
            currentProcess->waitingTime = currentProcess->turnaroundTime - currentProcess->burstTime;
            
            totalWaitingTime += currentProcess->waitingTime;
            totalTurnaroundTime += currentProcess->turnaroundTime;
            totalResponseTime += currentProcess->responseTime;
            
            completedProcesses++;
            currentProcess = nullptr;
            
            if (!readyQueue.empty()) {
                currentTime += contextSwitchTime;
            }
        }
        
        if (currentProcess == nullptr && !readyQueue.empty()) {
            currentProcess = const_cast<Process*>(readyQueue.top());
            readyQueue.pop();
            currentProcess->state = RUNNING;
            
            currentProcess->responseTime = currentTime - currentProcess->arrivalTime;
            currentProcess->startTime = currentTime;
            currentProcess->firstExecution = false;
        }
        
        deque<Process*> queueCopy;
        auto tempQueue = readyQueue;
        while (!tempQueue.empty()) {
            queueCopy.push_back(const_cast<Process*>(tempQueue.top()));
            tempQueue.pop();
        }
        visualizeQueue(queueCopy, currentProcess);
        
        // Display detailed state every 5 time units
        if (currentTime % 5 == 0) {
            displayProcessStates();
        }
        
        if (currentProcess != nullptr) {
            currentProcess->remainingTime--;
        }
        
        currentTime++;
        
        if (currentProcess == nullptr && readyQueue.empty() && nextArrival < processes.size()) {
            currentTime = processes[nextArrival].arrivalTime;
        }
    }
    
    displayMetrics();
}

// ============== Multilevel Queue Implementation ==============
MultilevelQueueScheduler::MultilevelQueueScheduler(int quantum, int contextSwitch)
    : Scheduler(contextSwitch), timeQuantum(quantum) {}

void MultilevelQueueScheduler::schedule() {
    cout << "\n========== Multilevel Queue Scheduling ==========\n";
    cout << "System Queue (Priority 0-1) > Interactive Queue (Priority 2-3) > Batch Queue (Priority 4-5)\n";
    
    Process* currentProcess = nullptr;
    int timeSlice = 0;
    
    sort(processes.begin(), processes.end(), 
         [](const Process& a, const Process& b) { return a.arrivalTime < b.arrivalTime; });
    
    size_t nextArrival = 0;
    
    while (completedProcesses < processes.size()) {
        while (nextArrival < processes.size() && 
               processes[nextArrival].arrivalTime <= currentTime) {
            processes[nextArrival].state = READY;
            
            if (processes[nextArrival].priority <= 1) {
                systemQueue.push_back(&processes[nextArrival]);
            } else if (processes[nextArrival].priority <= 3) {
                interactiveQueue.push_back(&processes[nextArrival]);
            } else {
                batchQueue.push_back(&processes[nextArrival]);
            }
            nextArrival++;
        }
        
        if (currentProcess != nullptr) {
            if (currentProcess->remainingTime == 0) {
                currentProcess->state = TERMINATED;
                currentProcess->completionTime = currentTime;
                currentProcess->turnaroundTime = currentTime - currentProcess->arrivalTime;
                currentProcess->waitingTime = currentProcess->turnaroundTime - currentProcess->burstTime;
                
                totalWaitingTime += currentProcess->waitingTime;
                totalTurnaroundTime += currentProcess->turnaroundTime;
                totalResponseTime += currentProcess->responseTime;
                
                completedProcesses++;
                currentProcess = nullptr;
                timeSlice = 0;
                
                if (!systemQueue.empty() || !interactiveQueue.empty() || !batchQueue.empty()) {
                    currentTime += contextSwitchTime;
                }
            } else if (timeSlice >= timeQuantum && currentProcess->priority > 1) {
                currentProcess->state = READY;
                
                if (currentProcess->priority <= 3) {
                    interactiveQueue.push_back(currentProcess);
                } else {
                    batchQueue.push_back(currentProcess);
                }
                
                currentProcess = nullptr;
                timeSlice = 0;
                currentTime += contextSwitchTime;
            }
        }
        
        if (currentProcess == nullptr) {
            if (!systemQueue.empty()) {
                currentProcess = systemQueue.front();
                systemQueue.pop_front();
            } else if (!interactiveQueue.empty()) {
                currentProcess = interactiveQueue.front();
                interactiveQueue.pop_front();
            } else if (!batchQueue.empty()) {
                currentProcess = batchQueue.front();
                batchQueue.pop_front();
            }
            
            if (currentProcess != nullptr) {
                currentProcess->state = RUNNING;
                
                if (currentProcess->firstExecution) {
                    currentProcess->responseTime = currentTime - currentProcess->arrivalTime;
                    currentProcess->startTime = currentTime;
                    currentProcess->firstExecution = false;
                }
                timeSlice = 0;
            }
        }
        
        cout << "\n[Time " << currentTime << "]";
        cout << " CPU: ";
        if (currentProcess) {
            cout << "P" << currentProcess->pid << "(rem:" << currentProcess->remainingTime << ")";
        } else {
            cout << "IDLE";
        }
        
        cout << "\n  System Q: [";
        for (size_t i = 0; i < systemQueue.size(); i++) {
            cout << "P" << systemQueue[i]->pid << "(rem:" << systemQueue[i]->remainingTime << ")";
            if (i < systemQueue.size() - 1) cout << ", ";
        }
        cout << "]";
        
        cout << " | Interactive Q: [";
        for (size_t i = 0; i < interactiveQueue.size(); i++) {
            cout << "P" << interactiveQueue[i]->pid << "(rem:" << interactiveQueue[i]->remainingTime << ")";
            if (i < interactiveQueue.size() - 1) cout << ", ";
        }
        cout << "]";
        
        cout << " | Batch Q: [";
        for (size_t i = 0; i < batchQueue.size(); i++) {
            cout << "P" << batchQueue[i]->pid << "(rem:" << batchQueue[i]->remainingTime << ")";
            if (i < batchQueue.size() - 1) cout << ", ";
        }
        cout << "]\n";
        
        // Display detailed state every 5 time units
        if (currentTime % 5 == 0) {
            displayProcessStates();
        }
        
        if (currentProcess != nullptr) {
            currentProcess->remainingTime--;
            timeSlice++;
        }
        
        currentTime++;
        
        if (currentProcess == nullptr && systemQueue.empty() && 
            interactiveQueue.empty() && batchQueue.empty() && 
            nextArrival < processes.size()) {
            currentTime = processes[nextArrival].arrivalTime;
        }
    }
    
    displayMetrics();
}

// ============== Multilevel Feedback Queue Implementation ==============
MultilevelFeedbackQueueScheduler::MultilevelFeedbackQueueScheduler(int contextSwitch)
    : Scheduler(contextSwitch) {}

void MultilevelFeedbackQueueScheduler::schedule() {
    cout << "\n========== Multilevel Feedback Queue Scheduling ==========\n";
    cout << "Q0 (quantum=8) > Q1 (quantum=16) > Q2 (FCFS)\n";
    
    Process* currentProcess = nullptr;
    int timeSlice = 0;
    int currentQueueLevel = -1;
    
    sort(processes.begin(), processes.end(), 
         [](const Process& a, const Process& b) { return a.arrivalTime < b.arrivalTime; });
    
    size_t nextArrival = 0;
    
    while (completedProcesses < processes.size()) {
        while (nextArrival < processes.size() && 
               processes[nextArrival].arrivalTime <= currentTime) {
            processes[nextArrival].state = READY;
            queue0.push_back(&processes[nextArrival]);
            processQueue[processes[nextArrival].pid] = 0;
            nextArrival++;
        }
        
        if (currentProcess != nullptr) {
            if (currentProcess->remainingTime == 0) {
                currentProcess->state = TERMINATED;
                currentProcess->completionTime = currentTime;
                currentProcess->turnaroundTime = currentTime - currentProcess->arrivalTime;
                currentProcess->waitingTime = currentProcess->turnaroundTime - currentProcess->burstTime;
                
                totalWaitingTime += currentProcess->waitingTime;
                totalTurnaroundTime += currentProcess->turnaroundTime;
                totalResponseTime += currentProcess->responseTime;
                
                completedProcesses++;
                currentProcess = nullptr;
                timeSlice = 0;
                currentQueueLevel = -1;
                
                if (!queue0.empty() || !queue1.empty() || !queue2.empty()) {
                    currentTime += contextSwitchTime;
                }
            } else {
                bool demote = false;
                
                if (currentQueueLevel == 0 && timeSlice >= quantum0) {
                    demote = true;
                    processQueue[currentProcess->pid] = 1;
                    queue1.push_back(currentProcess);
                } else if (currentQueueLevel == 1 && timeSlice >= quantum1) {
                    demote = true;
                    processQueue[currentProcess->pid] = 2;
                    queue2.push_back(currentProcess);
                }
                
                if (demote) {
                    currentProcess->state = READY;
                    currentProcess = nullptr;
                    timeSlice = 0;
                    currentQueueLevel = -1;
                    currentTime += contextSwitchTime;
                }
            }
        }
        
        if (currentProcess == nullptr) {
            if (!queue0.empty()) {
                currentProcess = queue0.front();
                queue0.pop_front();
                currentQueueLevel = 0;
            } else if (!queue1.empty()) {
                currentProcess = queue1.front();
                queue1.pop_front();
                currentQueueLevel = 1;
            } else if (!queue2.empty()) {
                currentProcess = queue2.front();
                queue2.pop_front();
                currentQueueLevel = 2;
            }
            
            if (currentProcess != nullptr) {
                currentProcess->state = RUNNING;
                
                if (currentProcess->firstExecution) {
                    currentProcess->responseTime = currentTime - currentProcess->arrivalTime;
                    currentProcess->startTime = currentTime;
                    currentProcess->firstExecution = false;
                }
                timeSlice = 0;
            }
        }
        
        cout << "\n[Time " << currentTime << "]";
        cout << " CPU: ";
        if (currentProcess) {
            cout << "P" << currentProcess->pid << "(rem:" << currentProcess->remainingTime << ", Q" << currentQueueLevel << ")";
        } else {
            cout << "IDLE";
        }
        
        cout << "\n  Q0: [";
        for (size_t i = 0; i < queue0.size(); i++) {
            cout << "P" << queue0[i]->pid << "(rem:" << queue0[i]->remainingTime << ")";
            if (i < queue0.size() - 1) cout << ", ";
        }
        cout << "]";
        
        cout << " | Q1: [";
        for (size_t i = 0; i < queue1.size(); i++) {
            cout << "P" << queue1[i]->pid << "(rem:" << queue1[i]->remainingTime << ")";
            if (i < queue1.size() - 1) cout << ", ";
        }
        cout << "]";
        
        cout << " | Q2: [";
        for (size_t i = 0; i < queue2.size(); i++) {
            cout << "P" << queue2[i]->pid << "(rem:" << queue2[i]->remainingTime << ")";
            if (i < queue2.size() - 1) cout << ", ";
        }
        cout << "]\n";
        
        // Display detailed state every 5 time units
        if (currentTime % 5 == 0) {
            displayProcessStates();
        }
        
        if (currentProcess != nullptr) {
            currentProcess->remainingTime--;
            timeSlice++;
        }
        
        currentTime++;
        
        if (currentProcess == nullptr && queue0.empty() && 
            queue1.empty() && queue2.empty() && 
            nextArrival < processes.size()) {
            currentTime = processes[nextArrival].arrivalTime;
        }
    }
    
    displayMetrics();
}