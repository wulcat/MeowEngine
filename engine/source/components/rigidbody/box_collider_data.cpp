//
// Created by Akira Mujawar on 30/08/24.
//

#include "box_collider_data.hpp"


using namespace MeowEngine::entity;

BoxColliderData::BoxColliderData()
{
    //physx::PxTransform transform = physx::PxTransform(physx::PxVec3(0,10,0));
    Geometry = physx::PxBoxGeometry(physx::PxVec3(0.5f, 0.5f, 0.5f));
}

physx::PxGeometry& BoxColliderData::GetGeometry() {
    return Geometry;
}
