//
// Created by Akira Mujawar on 06/07/22.
//

#ifndef VULKAN_ENGINE_ENGINE_HPP
#define VULKAN_ENGINE_ENGINE_HPP

#pragma once

#include <frame_rate_counter.hpp>
#include "internal_ptr.hpp"
#include "emscripten_wrapper.hpp"

#include "thread"
#include "log.hpp" // temp
#include "entt_reflection.hpp"

using namespace std;

namespace MeowEngine {
    struct Application {
        Application();
        virtual ~Application() = default;

        void StartApplication();
        bool LoopApplication();
//        virtual void OnWindowResized() = 0;

        virtual bool Input(const float& deltaTime) = 0;
        virtual void FixedUpdate(const float& inFixedDeltaTime) = 0;
        virtual void Update(const float& deltaTime) = 0;
        virtual void Render() = 0;

        std::atomic<bool> IsApplicationRunning;
        std::thread MainThread;
        std::thread RenderThread;

//        void MainLoop()
//        {
//            while(IsApplicationRunning)
//            {
//                float deltaTime = InternalPointer->TimeStep();
//                FpsCounter.frameStart();
//                // If Input() returns false - close the application
//                if(!Input(deltaTime)) {
//                    IsApplicationRunning = false;
//                }
//
//                if(InternalPointer->FixedStep()) {
//                    FixedUpdate(InternalPointer->Accumulator + InternalPointer->FixedTimeStep);
//                }
//
//                Update(deltaTime);
//
//                std::this_thread::sleep_for(std::chrono::milliseconds(10));
//            }
//
//            // wait for all threads to complete
//        }

        void RenderLoop()
        {
            while(IsApplicationRunning)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                Render();

            }
        }

        // this will go from 0 to 2
        // for now only 0 to 1 for rendering & main thread
//        static int mainThreadBufferIndex;

        static FrameRateCounter FpsCounter;

    private:
        struct Internal;
        MeowEngine::internal_ptr<Internal> InternalPointer;
    };
} // namespace MeowEngine


#endif //VULKAN_ENGINE_ENGINE_HPP
