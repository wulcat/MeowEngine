//
// Created by Akira Mujawar on 30/08/24.
//

#include "box_collider_data.hpp"


using namespace physicat::entity;

BoxColliderData::BoxColliderData(const entity::Transform3DComponent *inTransform)
{
    //physx::PxTransform transform = physx::PxTransform(physx::PxVec3(0,10,0));
    Geometry = physx::PxBoxGeometry(physx::PxVec3(0.5f, 0.5f, 0.5f));
}

physx::PxGeometry& BoxColliderData::GetGeometry() {
    return Geometry;
}
