//
// Created by Akira Mujawar on 03/02/25.
//

#ifndef MEOWENGINE_THREAD_PAUSE_HPP
#define MEOWENGINE_THREAD_PAUSE_HPP

#include "condition_variable"
#include "mutex"

namespace MeowEngine {

    class ThreadPause {
    public:
        ThreadPause() : ShouldWait(false) {}

        void Pause() {
            std::unique_lock<std::mutex> lock(waitMutex);
            ShouldWait = true;
            waitCondition.wait(
                lock,
                [this](){
                    return !ShouldWait;
                }
            );
        }

        void Release() {
            if(ShouldWait) {
                ShouldWait = false;
                waitCondition.notify_all();
            }
        }

    private:
        bool ShouldWait;

        std::mutex waitMutex;
        std::condition_variable waitCondition;
    };

} // MeowEngine

#endif //MEOWENGINE_THREAD_PAUSE_HPP
