//
// Created by Akira Mujawar on 20/01/25.
//

#include "opengl_physics_thread.hpp"
#include "application_test.hpp"

using namespace std;

namespace MeowEngine {

    void OpenGLPhysicsThread::StartThread(MeowEngine::ApplicationTest& inApplication) {

        PhysicsThread = std::thread(&MeowEngine::OpenGLPhysicsThread::PhysicsThreadLoop, this, std::ref(inApplication));
    }
    void OpenGLPhysicsThread::EndThread() {
        PhysicsThread.join();
    }

    void OpenGLPhysicsThread::PhysicsThreadLoop(MeowEngine::ApplicationTest& inApplication) {
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