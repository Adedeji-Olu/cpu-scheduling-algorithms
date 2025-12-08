# User Manual

## Building the Project
```bash
gmake build    # Compile
gmake debug    # Compile with debug symbols
gmake test     # Run all tests
gmake clean    # Remove build files
```

## Running the Simulator
```bash
./bin/scheduler
```

### Menu Options
1. **Round Robin** - Fair time-sharing (choose quantum: 4-8 recommended)
2. **Preemptive Priority** - High priority preempts low
3. **Non-Preemptive Priority** - No preemption once started
4. **Multilevel Queue** - Three-tier queues (System/Interactive/Batch)
5. **Multilevel Feedback Queue** - Adaptive scheduling
6. **Run All** - Compare all algorithms
7. **Custom Processes** - Define your own process set

## Understanding Output
```
[Time 5] CPU: P1(rem:6) | Ready Queue: [P2(rem:5), P3(rem:8)]
```
- `rem:X` = Remaining CPU time
- Process state table shows: NEW, READY, RUN, DONE

## Performance Metrics
- **Waiting Time**: Time in ready queue (lower is better)
- **Turnaround Time**: Total time in system (lower is better)
- **Response Time**: Time until first CPU access (lower is better)
