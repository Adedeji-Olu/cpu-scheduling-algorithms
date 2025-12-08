# CPU Scheduling Algorithms Simulator

A comprehensive CPU scheduling simulator implementing multiple scheduling algorithms with performance analysis and real-time visualization capabilities.

## 👥 Team Members

- **Adedeji Olu** - Project Lead & Implementation
- **Almamy Sow** - Testing & Documentation
- **Brionna Nunn** - Performance Analysis & Presentation

## 📋 Table of Contents

- [Features](#features)
- [Build Instructions](#build-instructions)
- [Usage Examples](#usage-examples)
- [Design Decisions](#design-decisions)
- [Known Limitations](#known-limitations)
- [Testing](#testing)
- [Performance Analysis](#performance-analysis)
- [References](#references)

## ✨ Features

### Implemented Scheduling Algorithms

1. **Round Robin (RR)** - Time-sharing with configurable quantum
2. **Preemptive Priority** - Dynamic priority-based preemption with aging
3. **Non-Preemptive Priority** - Static priority scheduling
4. **Multilevel Queue (MLQ)** - Three-tier queue system (System/Interactive/Batch)
5. **Multilevel Feedback Queue (MLFQ)** - Adaptive priority scheduling

### Key Capabilities

- ✅ Real-time process state visualization
- ✅ Comprehensive performance metrics (waiting, turnaround, response time)
- ✅ Context switching overhead modeling
- ✅ Process Control Block (PCB) simulation
- ✅ Interactive command-line interface
- ✅ Google Test framework integration
- ✅ Professional build system with Makefile

## 🛠️ Build Instructions

### Prerequisites

**FreeBSD:**
```bash
pkg install git gmake googletest
```

**Linux (Ubuntu/Debian):**
```bash
sudo apt-get install build-essential git libgtest-dev cmake
```

**macOS:**
```bash
brew install git cmake googletest
```

### Compilation
```bash
# Clone the repository
git clone https://github.com/Adedeji-Olu/cpu-scheduling-algorithms.git
cd cpu-scheduling-algorithms

# Build the project
gmake build          # or 'make build' on Linux/macOS

# Run tests
gmake test

# Run the simulator
./bin/scheduler
```

### Available Make Targets
```bash
gmake build        # Compile with optimizations (-O2)
gmake debug        # Compile with debug symbols (-g)
gmake test         # Run all tests (unit + system)
gmake unit-test    # Run unit tests only
gmake system-test  # Run system tests only
gmake clean        # Remove build artifacts
gmake install      # Install to /usr/local/bin
gmake help         # Show all available targets
```

## 📖 Usage Examples

### Running the Simulator
```bash
./bin/scheduler
```

### Interactive Menu
```
===== CPU Scheduling Simulator =====
1. Round Robin Scheduling
2. Preemptive Priority Scheduling
3. Non-Preemptive Priority Scheduling
4. Multilevel Queue Scheduling
5. Multilevel Feedback Queue Scheduling
6. Run All Algorithms (Comparison)
7. Add Custom Processes
0. Exit
```

### Example: Round Robin
```
Select algorithm: 1
Enter time quantum: 4

[Time 0] CPU: P1(rem:10) | Ready Queue: [P2(rem:5), P3(rem:8)]
[Time 4] CPU: P2(rem:5) | Ready Queue: [P3(rem:8), P1(rem:6)]
...

Average Waiting Time: 6.50 ms
Average Turnaround Time: 14.00 ms
Average Response Time: 2.00 ms
```

### Running Tests
```bash
# Run all tests
gmake test

# View test output
./bin/unit_tests --gtest_color=yes
./bin/system_tests --gtest_color=yes
```

## 🏗️ Design Decisions

### Architecture

- **Object-Oriented Design**: Base `Scheduler` class with algorithm-specific subclasses
- **Process Control Block**: Complete PCB simulation with all process states
- **Modular Structure**: Separated concerns (Process, Scheduler, Algorithms)

### Algorithm Implementations

1. **Round Robin**: FIFO queue with time quantum preemption
2. **Priority Scheduling**: Min-heap for efficient priority management
3. **Multilevel Queue**: Three separate queues with fixed priority levels
4. **MLFQ**: Dynamic queue promotion/demotion based on behavior

### Key Technical Choices

- **C++17 Standard**: Modern C++ features for cleaner code
- **Google Test Framework**: Industry-standard testing
- **Context Switching**: Configurable overhead (0-5 time units)
- **Aging Mechanism**: Priority boost every 10 time units to prevent starvation

## ⚠️ Known Limitations

### Current Limitations

1. **Single CPU**: Simulator assumes single-processor system
2. **No I/O Simulation**: Pure CPU scheduling (no I/O wait states)
3. **Fixed Process Set**: Dynamic process arrival not fully implemented
4. **Memory Constraints**: Large process counts (>1000) may slow visualization

### Future Enhancements

- Multi-processor scheduling support
- I/O burst modeling
- Real-time priority inheritance
- Interactive Gantt chart generation
- Save/load simulation states

## 🧪 Testing

### Test Coverage

- **Unit Tests**: 4+ tests covering individual components
- **System Tests**: 4+ end-to-end algorithm validation tests
- **Performance Benchmarks**: Scaling tests with 100+ processes

### Running Tests
```bash
# All tests
gmake test

# Unit tests only
gmake unit-test

# System tests only
gmake system-test

# Verbose output
./bin/unit_tests --gtest_verbose
```

### Test Results
```
[==========] Running 8 tests from 4 test suites.
[----------] Global test environment set-up.
...
[  PASSED  ] 8 tests.
```

## 📊 Performance Analysis

### Algorithm Comparison (10 processes, varying burst times)

| Algorithm | Avg Waiting Time | Avg Turnaround | Best Use Case |
|-----------|------------------|----------------|---------------|
| Round Robin (q=4) | 12.4 ms | 18.8 ms | Interactive systems |
| Preemptive Priority | 10.2 ms | 16.6 ms | Real-time tasks |
| Non-Preemptive | 13.1 ms | 19.5 ms | Batch processing |
| Multilevel Queue | 14.3 ms | 20.7 ms | Mixed workloads |
| MLFQ | 11.8 ms | 18.2 ms | Unknown behavior |

### Key Findings

1. **Preemptive Priority** achieves lowest average waiting time
2. **Context switching** overhead: 5-17% of total execution time
3. **Quantum selection** critical: 4-8 time units optimal for Round Robin
4. **MLFQ** adapts well to varying workload patterns

See [doc/PERFORMANCE_ANALYSIS.md](doc/PERFORMANCE_ANALYSIS.md) for detailed results.

## 📚 References

1. Silberschatz, A., Galvin, P. B., & Gagne, G. (2018). *Operating System Concepts* (10th ed.). Wiley.
2. Tanenbaum, A. S., & Bos, H. (2014). *Modern Operating Systems* (4th ed.). Pearson.
3. FreeBSD Scheduler Documentation: https://docs.freebsd.org/
4. Google Test Documentation: https://google.github.io/googletest/

## 📄 License

This project is part of an academic course assignment.

## 🤝 Contributing

This is an academic project. For questions or suggestions, please open an issue.

---

**Course:** Operating Systems  
**Instructor:** Prof. Alfa Anyadoro  
**Institution:** Bowie State University  
**Semester:** Fall 2025
