//
// Created by Akira Mujawar on 22/01/25.
//

#ifndef MEOWENGINE_SCOPED_ATOMIC_ACCESS_HPP
#define MEOWENGINE_SCOPED_ATOMIC_ACCESS_HPP

#include "functional"
#include "atomic"
#include "log.hpp"

namespace MeowEngine {

    template<typename Type>
    struct ScopedAtomicAccess {
        ScopedAtomicAccess(std::atomic<Type>& inAtomic, std::function<void()> inOnEnd)
                : AtomicVariable(inAtomic)
                , OnAccessEnd(std::move(inOnEnd)) {
            MeowEngine::Log("Scoped Access", "Created");
        }

        ~ScopedAtomicAccess() {
            OnAccessEnd();
            MeowEngine::Log("Scoped Access", "Destroyed");
        }

    public:
        std::atomic<Type>& Get() {
            return AtomicVariable;
        }

    private:
        std::atomic<Type>& AtomicVariable;
        std::function<void()> OnAccessEnd;
    };

} // MeowEngine

#endif //MEOWENGINE_SCOPED_ATOMIC_ACCESS_HPP
