//
// Created by Akira Mujawar on 30/08/24.
//

#include "rigidbody_component.hpp"

physicat::entity::RigidbodyComponent::RigidbodyComponent() {

}

void physicat::entity::RigidbodyComponent::SetPhysicsBody(physx::PxRigidDynamic *inBody) {
    DynamicBody = inBody;
}

void RigidbodyComponent::UpdateTransform(Transform3DComponent &inTransform) {
    auto pose = DynamicBody->getGlobalPose();
    inTransform.Position.X = pose.p.x;
    inTransform.Position.Y = pose.p.y;
    inTransform.Position.Z = pose.p.z;
}

void RigidbodyComponent::OverrideTransform(Transform3DComponent &inTransform) {
    DynamicBody->setGlobalPose(physx::PxTransform(inTransform.Position.X,inTransform.Position.Y,inTransform.Position.Z));
}

