/**
 * Unit Tests for CPU Scheduler - Google Test Framework
 */
#include "scheduler.h"
#include <gtest/gtest.h>

TEST(ProcessTest, Creation) {
    Process p(1, 0, 10, 2);
    EXPECT_EQ(p.pid, 1);
    EXPECT_EQ(p.arrivalTime, 0);
    EXPECT_EQ(p.burstTime, 10);
    EXPECT_EQ(p.priority, 2);
}

TEST(ProcessTest, StateTransition) {
    Process p(1, 0, 5, 1);
    EXPECT_EQ(p.state, ProcessState::NEW);
    p.state = ProcessState::READY;
    EXPECT_EQ(p.state, ProcessState::READY);
}

TEST(RoundRobinTest, BasicExecution) {
    RoundRobinScheduler rr(4, 0);
    rr.addProcess(Process(1, 0, 10, 1));
    rr.schedule();
    EXPECT_GT(rr.getAverageTurnaroundTime(), 0);
}

TEST(SchedulerTest, MetricsCalculation) {
    RoundRobinScheduler rr(4, 0);
    rr.addProcess(Process(1, 0, 10, 1));
    rr.addProcess(Process(2, 0, 5, 1));
    rr.schedule();
    
    EXPECT_GT(rr.getAverageWaitingTime(), 0);
    EXPECT_GT(rr.getAverageTurnaroundTime(), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
