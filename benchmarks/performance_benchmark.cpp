/**
 * Performance Benchmarks for CPU Scheduler
 * Measures execution time and efficiency of algorithms
 */

#include "scheduler.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <vector>

using namespace std;
using namespace chrono;

// Benchmark result structure
struct BenchmarkResult {
    string algorithm;
    double executionTime;
    double avgWaiting;
    double avgTurnaround;
    int numProcesses;
};

// Redirect cout to suppress output during benchmarking
class CoutRedirect {
    stringstream buffer;
    streambuf* old;
public:
    CoutRedirect() : old(cout.rdbuf(buffer.rdbuf())) {}
    ~CoutRedirect() { cout.rdbuf(old); }
};

void printBenchmarkHeader() {
    cout << "\n========== Performance Benchmark Results ==========\n";
    cout << left << setw(25) << "Algorithm"
         << right << setw(12) << "Processes"
         << setw(15) << "Exec Time(ms)"
         << setw(15) << "Avg Wait(ms)"
         << setw(15) << "Avg Turn(ms)" << endl;
    cout << string(82, '-') << endl;
}

void printBenchmarkResult(const BenchmarkResult& result) {
    cout << left << setw(25) << result.algorithm
         << right << setw(12) << result.numProcesses
         << setw(15) << fixed << setprecision(2) << result.executionTime
         << setw(15) << result.avgWaiting
         << setw(15) << result.avgTurnaround << endl;
}

BenchmarkResult benchmarkAlgorithm(Scheduler* scheduler, 
                                   const string& name,
                                   const vector<Process>& processes) {
    BenchmarkResult result;
    result.algorithm = name;
    result.numProcesses = processes.size();
    
    // Add processes
    for (const auto& p : processes) {
        scheduler->addProcess(p);
    }
    
    // Benchmark execution
    CoutRedirect redirect;
    
    auto start = high_resolution_clock::now();
    scheduler->schedule();
    auto end = high_resolution_clock::now();
    
    auto duration = duration_cast<microseconds>(end - start);
    result.executionTime = duration.count() / 1000.0;
    result.avgWaiting = scheduler->getAverageWaitingTime();
    result.avgTurnaround = scheduler->getAverageTurnaroundTime();
    
    return result;
}

vector<Process> generateProcesses(int count) {
    vector<Process> processes;
    for (int i = 0; i < count; i++) {
        int arrival = i / 2;
        int burst = 5 + (i % 10);
        int priority = i % 5;
        processes.push_back(Process(i + 1, arrival, burst, priority));
    }
    return processes;
}

int main() {
    cout << "\n========================================\n";
    cout << "  CPU Scheduler Performance Benchmark\n";
    cout << "========================================\n";
    
    vector<int> testSizes = {10, 25, 50};
    
    for (int size : testSizes) {
        cout << "\nBenchmarking with " << size << " processes...\n";
        
        auto processes = generateProcesses(size);
        vector<BenchmarkResult> results;
        
        // Benchmark Round Robin
        {
            RoundRobinScheduler* rr = new RoundRobinScheduler(4, 1);
            results.push_back(benchmarkAlgorithm(rr, "Round Robin (q=4)", processes));
            delete rr;
        }
        
        // Benchmark Preemptive Priority
        {
            PreemptivePriorityScheduler* pp = new PreemptivePriorityScheduler(1);
            results.push_back(benchmarkAlgorithm(pp, "Preemptive Priority", processes));
            delete pp;
        }
        
        // Benchmark Non-Preemptive Priority
        {
            NonPreemptivePriorityScheduler* np = new NonPreemptivePriorityScheduler(1);
            results.push_back(benchmarkAlgorithm(np, "Non-Preemptive Priority", processes));
            delete np;
        }
        
        // Benchmark Multilevel Queue
        {
            MultilevelQueueScheduler* mq = new MultilevelQueueScheduler(4, 1);
            results.push_back(benchmarkAlgorithm(mq, "Multilevel Queue", processes));
            delete mq;
        }
        
        // Benchmark MLFQ
        {
            MultilevelFeedbackQueueScheduler* mfq = new MultilevelFeedbackQueueScheduler(1);
            results.push_back(benchmarkAlgorithm(mfq, "MLFQ", processes));
            delete mfq;
        }
        
        // Print results
        printBenchmarkHeader();
        for (const auto& result : results) {
            printBenchmarkResult(result);
        }
        cout << string(82, '=') << "\n";
    }
    
    cout << "\nBenchmark completed successfully!\n" << endl;
    
    return 0;
}
