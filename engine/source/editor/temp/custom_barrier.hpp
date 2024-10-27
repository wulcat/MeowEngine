//
// Created by Akira Mujawar on 27/10/24.
//

#ifndef PHYSICAT_CUSTOM_BARRIER_HPP
#define PHYSICAT_CUSTOM_BARRIER_HPP

#include "mutex"
#include "thread"
#include "condition_variable"

class CustomBarrier {
public:
    CustomBarrier(int totalThreadCount)
    : TotalThreadCount(totalThreadCount) {}

    void Wait() {
        // prevent race condition by blocking till out of scope
        std::unique_lock<std::mutex> lock(waitMutex);

        // add count to total waiting thread
        WaitThreadCount++;
        ShouldThreadWait = true;

        // when both physics and render thread are waiting
        if(WaitThreadCount == TotalThreadCount) {
            // we all threads are waiting, tell them to unpause/ continue
            ShouldThreadWait = false;
            waitCondition.notify_all();
        }
        else {
            // make all threads wait for frame to finish
            waitCondition.wait(
                lock,
                [this] {
                    return !ShouldThreadWait;
                }
            );
        }

        // remove waiting thread count back to 0
        WaitThreadCount--;
    }

private:
    int TotalThreadCount;
    int WaitThreadCount;
    bool ShouldThreadWait;

    std::mutex waitMutex;
    std::condition_variable waitCondition;
};


#endif //PHYSICAT_CUSTOM_BARRIER_HPP
