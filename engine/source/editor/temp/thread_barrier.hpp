//
// Created by Akira Mujawar on 27/10/24.
//

#ifndef PHYSICAT_THREAD_BARRIER_HPP
#define PHYSICAT_THREAD_BARRIER_HPP

#include "mutex"
#include "thread"
#include "condition_variable"
#include "tracy_wrapper.hpp"

class ThreadBarrier {
public:
    ThreadBarrier(int totalThreadCount)
    : TotalThreadCount(totalThreadCount) {}

    void Wait() {
        PT_PROFILE_SCOPE;
        if(ShouldEnd)
        {
            return;
        }

        // prevent race condition by blocking till out of scope
        std::unique_lock<std::mutex> lock(waitMutex);

        // add count to total waiting thread
        WaitThreadCount++;
        ShouldThreadWait = true;

        // when both physics and render thread are waiting
        if(WaitThreadCount == TotalThreadCount) {
            // we all threads are waiting, tell them to unpause/ continue
            Release();
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

    /**
     * releases waiting threads on demand
     */
    void Release() {
        ShouldThreadWait = false;
        waitCondition.notify_all();
    }

    void End()
    {
        ShouldEnd = true;
        Release();
    }

private:
    int TotalThreadCount;
    int WaitThreadCount;
    bool ShouldThreadWait;
    bool ShouldEnd;

    std::mutex waitMutex;
    std::condition_variable waitCondition;
};


#endif //PHYSICAT_THREAD_BARRIER_HPP
