//
// Created by Akira Mujawar on 20/01/25.
//

#include "physics_multi_thread.hpp"
#include "shared_thread_state.hpp"

using namespace std;

namespace MeowEngine {

    void PhysicsMultiThread::SetScene(std::shared_ptr<MeowEngine::SceneMultiThread> inScene) {
        Scene = inScene;
    }

    void PhysicsMultiThread::StartThread() {
        PhysicsThread = std::thread(&MeowEngine::PhysicsMultiThread::PhysicsThreadLoop, this);
    }
    void PhysicsMultiThread::EndThread() {
        PhysicsThread.join();
    }

    void PhysicsMultiThread::PhysicsThreadLoop() {
        MeowEngine::Log("Physics Thread", "Started");

        // Count active threads so we can close engine properly
        SharedState.ActiveWaitThread.GetAtomic()++;

        // Create Objects from scene (temp)
        Physics->Create();

        // Simulating Physics
        while(SharedState.IsAppRunning) {
            PT_PROFILE_SCOPE;

            FrameRateCounter->Calculate();

            Scene->AddEntitiesOnPhysicsSystem(Physics.get());
            Physics->Update(FrameRateCounter->DeltaTime);

            if(SharedState.SyncPointPhysicMutex.try_lock()) {
                Scene->SyncPhysicsBufferOnPhysicsSystem();
                SharedState.SyncPointPhysicMutex.unlock();
            }

            FrameRateCounter->LockFrameRate();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // Notify main thread, that thread count has changed
        SharedState.ActiveWaitThread.GetAtomicAndNotify().Get()--;

        MeowEngine::Log("Physics Thread", "Ended");
    }


} // MeowEngine