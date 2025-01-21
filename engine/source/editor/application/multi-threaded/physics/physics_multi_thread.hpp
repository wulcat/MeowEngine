//
// Created by Akira Mujawar on 20/01/25.
//

#ifndef MEOWENGINE_PHYSICS_MULTI_THREAD_HPP
#define MEOWENGINE_PHYSICS_MULTI_THREAD_HPP

#include "thread"
#include "frame_rate_counter.hpp"
#include "physx_physics_system.hpp"
//#include "tracy_wrapper.hpp"
#include "log.hpp"
//#include "application_test.hpp"

namespace MeowEngine {
    class OpenGLAppMultiThread;

    class PhysicsMultiThread {
    public:
        PhysicsMultiThread() {
            Physics = std::make_shared<MeowEngine::simulator::PhysXPhysics>();
            PhysicsThreadFrameRate = std::make_unique<FrameRateCounter>(50, 1); // per 0.02 sec
        }

        ~PhysicsMultiThread() {
            Physics.reset();
            PhysicsThreadFrameRate.reset();
        }

        std::thread PhysicsThread;
        std::shared_ptr<MeowEngine::simulator::Physics> Physics;
        std::unique_ptr<FrameRateCounter> PhysicsThreadFrameRate;

        void StartThread(MeowEngine::OpenGLAppMultiThread& inApplication);
        void EndThread();

        void PhysicsThreadLoop(MeowEngine::OpenGLAppMultiThread& inApplication);
//        void FixedUpdate(const float& inFixedDeltaTime) {
//            Scene->AddEntitiesOnPhysicsThread(Physics.get());
//            Physics->Update(inFixedDeltaTime);
//
//            if(SyncPhysicMutex.try_lock()) {
//                Scene->SyncPhysicsBufferOnPhysicsThread();
//                SyncPhysicMutex.unlock();
//            }
//        };
    };

} // MeowEngine

#endif //MEOWENGINE_PHYSICS_MULTI_THREAD_HPP
