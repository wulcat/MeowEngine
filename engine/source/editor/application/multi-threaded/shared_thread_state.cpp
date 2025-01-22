//
// Created by Akira Mujawar on 22/01/25.
//

#include "shared_thread_state.hpp"
#include "log.hpp"

namespace MeowEngine {
    SharedThreadState::SharedThreadState() {
        MeowEngine::Log("Shared State", "Created");
    }

    SharedThreadState::~SharedThreadState() {
        MeowEngine::Log("Shared State", "Destroyed");
    }
} // MeowEngine