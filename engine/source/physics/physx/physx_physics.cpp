//
// Created by Akira Mujawar on 12/08/24.
//

#include <log.hpp>
#include "physx_physics.hpp"

physicat::simulator::PhysXPhysics::PhysXPhysics() {
    gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
    gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, physx::PxTolerancesScale(), true, nullptr);

    // create scene
    physx::PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
    sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
    sceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
    sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

    gScene = gPhysics->createScene(sceneDesc);

    physicat::Log("Physics", "Constructed");
}

physicat::simulator::PhysXPhysics::~PhysXPhysics() {
    gScene->release();
    gPhysics->release();
    gFoundation->release();

    physicat::Log("Physics", "Destructed");
}

void physicat::simulator::PhysXPhysics::Create() {


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

void physicat::simulator::PhysXPhysics::Update(float inFixedDeltaTime) {
  //  for(int i = 0 ;i < 1000; ++i)
  //  {

  // physicat::Log("", std::to_string(body->getGlobalPose().p.y));
        gScene->simulate(0.02f);
        gScene->fetchResults(true);
  //  }
}

void physicat::simulator::PhysXPhysics::AddRigidbody(entity::Transform3DComponent &transform,
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