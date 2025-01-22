//
// Created by Akira Mujawar on 22/01/25.
//

#ifndef MEOWENGINE_SHARED_THREAD_STATE_HPP
#define MEOWENGINE_SHARED_THREAD_STATE_HPP

#include "queue"
#include "atomic"
#include "mutex"

#include "double_buffer.hpp"
#include "thread_barrier.hpp"
#include "thread_wait.hpp"

#include "sdl_wrapper.hpp"

namespace MeowEngine {
    struct SharedThreadState {
        SharedThreadState();
        ~SharedThreadState();

        std::atomic<bool> IsAppRunning;
        std::shared_ptr<ThreadBarrier> SyncPointStartRenderBarrier;
        std::shared_ptr<ThreadBarrier> SyncPointEndRenderBarrier;
        std::mutex SyncPointPhysicMutex;

        MeowEngine::ThreadWait<int> ActiveWaitThread;
        MeowEngine::DoubleBuffer<std::queue<SDL_Event>> SDLEventBuffer = MeowEngine::DoubleBuffer<std::queue<SDL_Event>>(); //
    };

} // MeowEngine

#endif //MEOWENGINE_SHARED_THREAD_STATE_HPP
