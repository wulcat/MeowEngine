//
// Created by Akira Mujawar on 20/01/25.
//

#include "physics_multi_thread.hpp"
#include "opengl_app_multi_thread.hpp"

using namespace std;

namespace MeowEngine {

    void PhysicsMultiThread::StartThread(MeowEngine::OpenGLAppMultiThread& inApplication) {

        PhysicsThread = std::thread(&MeowEngine::PhysicsMultiThread::PhysicsThreadLoop, this, std::ref(inApplication));
    }
    void PhysicsMultiThread::EndThread() {
        PhysicsThread.join();
    }

    void PhysicsMultiThread::PhysicsThreadLoop(MeowEngine::OpenGLAppMultiThread& inApplication) {
        MeowEngine::Log("Physics Thread", "Started");


        inApplication.ThreadCount++;
        Physics->Create();


        while(inApplication.IsApplicationRunning) {
            PT_PROFILE_SCOPE;

            PhysicsThreadFrameRate->Calculate();

            inApplication.Scene->AddEntitiesOnPhysicsThread(Physics.get());
            Physics->Update(PhysicsThreadFrameRate->DeltaTime);

            if(inApplication.SyncPhysicMutex.try_lock()) {
                inApplication.Scene->SyncPhysicsBufferOnPhysicsThread();
                inApplication.SyncPhysicMutex.unlock();
            }

            PhysicsThreadFrameRate->LockFrameRate();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        inApplication.ThreadCount--;

        MeowEngine::Log("Physics Thread", "Ended");
        inApplication.WaitForThreadEndCondition.notify_all();
    }


} // MeowEngine