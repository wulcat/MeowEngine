//
// Created by Akira Mujawar on 30/08/24.
//

#include "collider_component.hpp"

using namespace physicat::entity;

ColliderComponent::ColliderComponent(entity::ColliderType inType, entity::BoxColliderData* inData) {
    Type = inType;
    Data = inData;
}

physx::PxGeometry& ColliderComponent::GetGeometry() {
    return Data->GetGeometry();
}