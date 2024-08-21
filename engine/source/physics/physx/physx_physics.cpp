//
// Created by Akira Mujawar on 12/08/24.
//

#include "physx_physics.hpp"

physicat::simulator::PhysXPhysics::PhysXPhysics() {
    gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
    gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, physx::PxTolerancesScale(), true, nullptr);
    gScene = nullptr;
}

physicat::simulator::PhysXPhysics::~PhysXPhysics() {
    gScene->release();
    gPhysics->release();
    gFoundation->release();
}

void physicat::simulator::PhysXPhysics::Create() {
    // create scene
    physx::PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
    sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
    sceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
    sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

    gScene = gPhysics->createScene(sceneDesc);

    physx::PxRigidStatic* groundPlane = physx::PxCreatePlane(*gPhysics, physx::PxPlane(0,1,0,0), *gPhysics->createMaterial(0.0f, 0.0f, 0.6f));
    gScene->addActor(*groundPlane);

    physx::PxReal density = 1.0f;
    physx::PxTransform transform(physx::PxVec3(0,10,0));
    physx::PxBoxGeometry geometry(physx::PxVec3(0.5f, 0.5f, 0.5f));
    physx::PxRigidDynamic* dynamicBox = physx::PxCreateDynamic(*gPhysics, transform, geometry, *gPhysics->createMaterial(0.5f, 0.5f, 0.6f), density);
    gScene->addActor(*dynamicBox);
}

void physicat::simulator::PhysXPhysics::Update(float inFixedDeltaTime) {
  //  for(int i = 0 ;i < 1000; ++i)
  //  {
        gScene->simulate(inFixedDeltaTime);
        gScene->fetchResults(true);
  //  }
}