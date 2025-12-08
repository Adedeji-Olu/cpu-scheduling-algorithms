/**
 * System Tests - Google Test Framework
 */
#include "scheduler.h"
#include <gtest/gtest.h>

TEST(SystemTest, RoundRobinComplete) {
    RoundRobinScheduler rr(4, 0);
    rr.addProcess(Process(1, 0, 10, 2));
    rr.addProcess(Process(2, 0, 5, 1));
    rr.schedule();
    EXPECT_GT(rr.getAverageTurnaroundTime(), 0);
}

TEST(SystemTest, PreemptivePriority) {
    PreemptivePriorityScheduler pps(0);
    pps.addProcess(Process(1, 0, 10, 5));
    pps.addProcess(Process(2, 0, 5, 1));
    pps.schedule();
    EXPECT_GT(pps.getAverageTurnaroundTime(), 0);
}

TEST(SystemTest, NonPreemptivePriority) {
    NonPreemptivePriorityScheduler nps(0);
    nps.addProcess(Process(1, 0, 10, 3));
    nps.addProcess(Process(2, 0, 5, 1));
    nps.schedule();
    EXPECT_GT(nps.getAverageTurnaroundTime(), 0);
}

TEST(SystemTest, MultilevelQueue) {
    MultilevelQueueScheduler mqs(4, 0);
    mqs.addProcess(Process(1, 0, 10, 0));
    mqs.addProcess(Process(2, 0, 5, 5));
    mqs.schedule();
    EXPECT_GT(mqs.getAverageTurnaroundTime(), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
