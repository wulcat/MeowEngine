//
// Created by Akira Mujawar on 20/01/25.
//

#ifndef MEOWENGINE_PHYSICS_MULTI_THREAD_HPP
#define MEOWENGINE_PHYSICS_MULTI_THREAD_HPP

#include <scene.hpp>
#include "thread"
#include "frame_rate_counter.hpp"
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
            PhysicsThreadFrameRate = std::make_unique<FrameRateCounter>(50, 1); // per 0.02 sec
        }

        ~PhysicsMultiThread() {
            Physics.reset();
            PhysicsThreadFrameRate.reset();
        }

        std::thread PhysicsThread;
        std::shared_ptr<MeowEngine::simulator::PhysicsSystem> Physics;
        std::unique_ptr<FrameRateCounter> PhysicsThreadFrameRate;

        MeowEngine::SharedThreadState& SharedState;
        std::shared_ptr<MeowEngine::Scene> Scene;

        void SetScene(std::shared_ptr<MeowEngine::Scene> inScene);
        void StartThread();
        void EndThread();

    private:
        void PhysicsThreadLoop();
    };

} // MeowEngine

#endif //MEOWENGINE_PHYSICS_MULTI_THREAD_HPP
