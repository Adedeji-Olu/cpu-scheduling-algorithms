// src/main.cpp
// Main program with interactive menu

#include "scheduler.h"
#include <iostream>
#include <vector>
#include <memory>

using namespace std;

int main() {
    cout << "========================================\n";
    cout << "  Advanced CPU Scheduler Simulator\n";
    cout << "========================================\n\n";
    
    // Sample process set
    vector<Process> sampleProcesses = {
        Process(1, 0, 10, 2),   // PID, Arrival, Burst, Priority
        Process(2, 1, 5, 1),
        Process(3, 2, 8, 3),
        Process(4, 3, 4, 2),
        Process(5, 4, 3, 4)
    };
    
    cout << "Sample Process Set:\n";
    for (const auto& p : sampleProcesses) {
        cout << "  ";
        p.display();
        cout << "\n";
    }
    
    int choice;
    do {
        cout << "\n========== Scheduling Algorithms ==========\n";
        cout << "1. Round Robin\n";
        cout << "2. Preemptive Priority\n";
        cout << "3. Non-Preemptive Priority\n";
        cout << "4. Multilevel Queue\n";
        cout << "5. Multilevel Feedback Queue\n";
        cout << "6. Run All Algorithms (Comparison)\n";
        cout << "7. Add Custom Processes\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        
        if (choice >= 1 && choice <= 6) {
            unique_ptr<Scheduler> scheduler;
            
            switch (choice) {
                case 1: {
                    int quantum;
                    cout << "Enter time quantum: ";
                    cin >> quantum;
                    scheduler = make_unique<RoundRobinScheduler>(quantum, 1);
                    break;
                }
                case 2:
                    scheduler = make_unique<PreemptivePriorityScheduler>(1);
                    break;
                case 3:
                    scheduler = make_unique<NonPreemptivePriorityScheduler>(1);
                    break;
                case 4: {
                    int quantum;
                    cout << "Enter time quantum for interactive processes: ";
                    cin >> quantum;
                    scheduler = make_unique<MultilevelQueueScheduler>(quantum, 1);
                    break;
                }
                case 5:
                    scheduler = make_unique<MultilevelFeedbackQueueScheduler>(1);
                    break;
                case 6: {
                    // Run all algorithms for comparison
                    vector<unique_ptr<Scheduler>> schedulers;
                    schedulers.push_back(make_unique<RoundRobinScheduler>(4, 1));
                    schedulers.push_back(make_unique<PreemptivePriorityScheduler>(1));
                    schedulers.push_back(make_unique<NonPreemptivePriorityScheduler>(1));
                    schedulers.push_back(make_unique<MultilevelQueueScheduler>(4, 1));
                    schedulers.push_back(make_unique<MultilevelFeedbackQueueScheduler>(1));
                    
                    for (auto& sched : schedulers) {
                        for (const auto& p : sampleProcesses) {
                            sched->addProcess(p);
                        }
                        sched->schedule();
                        cout << "\n\n";
                    }
                    continue;
                }
            }
            
            if (scheduler) {
                for (const auto& p : sampleProcesses) {
                    scheduler->addProcess(p);
                }
                scheduler->schedule();
            }
        } else if (choice == 7) {
            sampleProcesses.clear();
            int n;
            cout << "Enter number of processes: ";
            cin >> n;
            
            for (int i = 0; i < n; i++) {
                int arrival, burst, priority;
                cout << "Process " << (i + 1) << ":\n";
                cout << "  Arrival Time: ";
                cin >> arrival;
                cout << "  Burst Time: ";
                cin >> burst;
                cout << "  Priority (0-5, lower=higher): ";
                cin >> priority;
                
                sampleProcesses.push_back(Process(i + 1, arrival, burst, priority));
            }
            
            cout << "\nCustom processes added successfully!\n";
        }
        
    } while (choice != 0);
    
    cout << "\nThank you for using the CPU Scheduler Simulator!\n";
    
    return 0;
}