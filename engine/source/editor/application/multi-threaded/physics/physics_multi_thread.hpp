//
// Created by Akira Mujawar on 20/01/25.
//

#ifndef MEOWENGINE_PHYSICS_MULTI_THREAD_HPP
#define MEOWENGINE_PHYSICS_MULTI_THREAD_HPP

#include "thread"
#include "frame_rate_counter.hpp"
#include "scene_multi_thread.hpp"

#include "physx_physics_system.hpp"

//#include "tracy_wrapper.hpp"
#include "log.hpp"
//#include "application_test.hpp"

namespace MeowEngine {

    class SharedThreadState;

    class PhysicsMultiThread {
    public:
        PhysicsMultiThread(MeowEngine::SharedThreadState& inState)
        : SharedState(inState) {
            Physics = std::make_shared<MeowEngine::simulator::PhysXPhysicsSystem>();
            FrameRateCounter = std::make_unique<MeowEngine::FrameRateCounter>(50, 1); // per 0.02 sec
        }

        ~PhysicsMultiThread() {
            Physics.reset();
            FrameRateCounter.reset();
        }

        void SetScene(std::shared_ptr<MeowEngine::SceneMultiThread> inScene);
        void StartThread();
        void EndThread();

    private:
        void PhysicsThreadLoop();

        std::thread PhysicsThread;

        std::shared_ptr<MeowEngine::simulator::PhysicsSystem> Physics;
        std::unique_ptr<MeowEngine::FrameRateCounter> FrameRateCounter;

        MeowEngine::SharedThreadState& SharedState;
        std::shared_ptr<MeowEngine::SceneMultiThread> Scene;
    };

} // MeowEngine

#endif //MEOWENGINE_PHYSICS_MULTI_THREAD_HPP
