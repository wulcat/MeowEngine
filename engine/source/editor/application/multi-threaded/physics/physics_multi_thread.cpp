//
// Created by Akira Mujawar on 20/01/25.
//

#include "physics_multi_thread.hpp"
#include "shared_thread_state.hpp"

using namespace std;

namespace MeowEngine {

    void PhysicsMultiThread::SetScene(std::shared_ptr<MeowEngine::Scene> inScene) {
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


        SharedState.ThreadCount++;
        Physics->Create();


        while(SharedState.IsApplicationRunning) {
            PT_PROFILE_SCOPE;

            PhysicsThreadFrameRate->Calculate();

            Scene->AddEntitiesOnPhysicsThread(Physics.get());
            Physics->Update(PhysicsThreadFrameRate->DeltaTime);

            if(SharedState.SyncPhysicMutex.try_lock()) {
                Scene->SyncPhysicsBufferOnPhysicsThread();
                SharedState.SyncPhysicMutex.unlock();
            }

            PhysicsThreadFrameRate->LockFrameRate();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        SharedState.ThreadCount--;

        MeowEngine::Log("Physics Thread", "Ended");
        SharedState.WaitForThreadEndCondition.notify_all();
    }


} // MeowEngine