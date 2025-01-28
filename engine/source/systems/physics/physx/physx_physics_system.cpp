//
// Created by Akira Mujawar on 12/08/24.
//

#include <log.hpp>
#include "physx_physics_system.hpp"

MeowEngine::simulator::PhysXPhysicsSystem::PhysXPhysicsSystem() {
    gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
    gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, physx::PxTolerancesScale(), true, nullptr);

    // create scene
    physx::PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
    sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
    sceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(0); // can't have more than 1 thread here for web
    sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

    gScene = gPhysics->createScene(sceneDesc);

    MeowEngine::Log("Physics", "Constructed");
}

MeowEngine::simulator::PhysXPhysicsSystem::~PhysXPhysicsSystem() {
    gScene->release();
    gPhysics->release();
    gFoundation->release();

    MeowEngine::Log("Physics", "Destructed");
}

void MeowEngine::simulator::PhysXPhysicsSystem::Create() {


    physx::PxRigidStatic* groundPlane = physx::PxCreatePlane(*gPhysics, physx::PxPlane(0,1,0,0), *gPhysics->createMaterial(0.0f, 0.0f, 0.6f));
    gScene->addActor(*groundPlane);
//groundPlane->getGlobalPose()
  //  physx::PxShape* test =  gPhysics->createShape(physx::PxBoxGeometry(), *gPhysics->createMaterial(0.5f, 0.5f, 0.6f));
  //  physx::PxCreateStatic(*gPhysics, physx::PxTransform(), *test);

//    physx::PxReal density = 1.0f;
//     testTransform = physx::PxTransform(physx::PxVec3(0,10,0));
//    physx::PxBoxGeometry geometry(physx::PxVec3(0.5f, 0.5f, 0.5f));
//    body = physx::PxCreateDynamic(*gPhysics, testTransform, geometry, *gPhysics->createMaterial(0.5f, 0.5f, 0.6f), density);
//
//    gScene->addActor(*body);



//    gScene->removeActor(*body);
}

void MeowEngine::simulator::PhysXPhysicsSystem::Update(float inFixedDeltaTime) {
  //  for(int i = 0 ;i < 1000; ++i)
  //  {

  // MeowEngine::Log("", std::to_string(body->getGlobalPose().p.y));
        gScene->simulate(inFixedDeltaTime);
        gScene->fetchResults(true);
  //  }
}

void MeowEngine::simulator::PhysXPhysicsSystem::AddRigidbody(entity::Transform3DComponent &transform,
                                                             entity::ColliderComponent &collider,
                                                             entity::RigidbodyComponent &rigidbody) {

    physx::PxTransform physicsTransform(physx::PxVec3(transform.Position.X,transform.Position.Y,transform.Position.Z));
    physx::PxReal density = 1.0f;
    physx::PxGeometry& geometry = collider.GetGeometry(); // has scale data as well
// transform has rotation and position data
    physx::PxRigidDynamic* actor = physx::PxCreateDynamic(*gPhysics, physicsTransform, geometry, *gPhysics->createMaterial(0.5f, 0.5f, 0.6f), density);

    rigidbody.SetPhysicsBody(actor);
    gScene->addActor(*actor);
}