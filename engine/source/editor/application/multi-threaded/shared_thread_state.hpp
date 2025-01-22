//
// Created by Akira Mujawar on 22/01/25.
//

#ifndef MEOWENGINE_SHARED_THREAD_STATE_HPP
#define MEOWENGINE_SHARED_THREAD_STATE_HPP

#include "atomic"
#include "condition_variable"
#include "mutex"
#include "double_buffer.hpp"
#include "queue"
#include "thread_barrier.hpp"
#include "thread_wait.hpp"

#include "sdl_wrapper.hpp"


namespace MeowEngine {

    struct SharedThreadState {
        SharedThreadState();
        ~SharedThreadState();

        std::atomic<bool> IsAppRunning;
        MeowEngine::ThreadWait<int> ActiveWaitThread;
        
        std::mutex SyncPhysicMutex; //

        MeowEngine::DoubleBuffer<std::queue<SDL_Event>> InputBuffer = MeowEngine::DoubleBuffer<std::queue<SDL_Event>>(); //

        // main thread ----------------

        std::shared_ptr<ThreadBarrier> ProcessThreadBarrier;
        std::shared_ptr<ThreadBarrier> SwapBufferThreadBarrier;
    };

} // MeowEngine

#endif //MEOWENGINE_SHARED_THREAD_STATE_HPP
