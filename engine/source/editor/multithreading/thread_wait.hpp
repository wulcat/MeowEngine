//
// Created by Akira Mujawar on 22/01/25.
//

#ifndef MEOWENGINE_THREAD_WAIT_HPP
#define MEOWENGINE_THREAD_WAIT_HPP

#include <utility>

#include "mutex"
#include "atomic"
#include "condition_variable"
#include "functional"
#include "scoped_atomic_access.hpp"


using namespace std;

namespace MeowEngine {
    template<typename Type>
    class ThreadWait {

    public:
        void Wait(std::function<bool(Type)> checkCondition) {
            std::unique_lock<std::mutex> lock(waitMutex);
            waitCondition.wait(
                lock,
                [this, &checkCondition] {
                    return checkCondition(atomicVariable);
                }
            );
        }

        std::atomic<Type>& GetAtomic() {
            return atomicVariable;
        }

        // RAII principle (Resource acquisition is initialization)
        MeowEngine::ScopedAtomicAccess<Type> GetAtomicAndNotify() {
            return MeowEngine::ScopedAtomicAccess<Type>(atomicVariable, [&]() {
                Notify();
            });
        }

        void Notify() {
            waitCondition.notify_all();
        }

    private:
        std::mutex waitMutex;
        std::condition_variable waitCondition;
        std::atomic<Type> atomicVariable;
    };

} // MeowEngine

#endif //MEOWENGINE_THREAD_WAIT_HPP
