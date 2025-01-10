//
// Created by Akira Mujawar on 30/08/24.
//

#include "collider_component.hpp"
#include <log.hpp>

using namespace physicat::entity;

void physicat::entity::ColliderComponent::Reflect(){
//    REGISTER_PROPERTY(RigidbodyComponent, Position, physicat::math::Vector3);

    physicat::Log("Reflected", "ColliderComponent");
}

ColliderComponent::ColliderComponent(entity::ColliderType inType, entity::BoxColliderData* inData) {
    Type = inType;
    Data = inData;
}

physx::PxGeometry& ColliderComponent::GetGeometry() {
    return Data->GetGeometry();
}