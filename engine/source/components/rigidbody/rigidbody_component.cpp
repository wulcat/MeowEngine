//
// Created by Akira Mujawar on 30/08/24.
//

#include "rigidbody_component.hpp"
#include "log.hpp"

void physicat::entity::RigidbodyComponent::Reflect(){
//    REGISTER_PROPERTY(RigidbodyComponent, Position, physicat::math::Vector3);

    physicat::Log("Reflected", "RigidbodyComponent");
}

physicat::entity::RigidbodyComponent::RigidbodyComponent() {

}

void physicat::entity::RigidbodyComponent::SetPhysicsBody(physx::PxRigidDynamic *inBody) {
    DynamicBody = inBody;
}

void RigidbodyComponent::UpdateTransform(Transform3DComponent &inTransform) {
    auto pose = DynamicBody->getGlobalPose();
    inTransform.Position.X = pose.p.x + Delta.X;
    inTransform.Position.Y = pose.p.y + Delta.Y;
    inTransform.Position.Z = pose.p.z + Delta.Z;

    Delta.X = 0;
    Delta.Y = 0;
    Delta.Z = 0;

    DynamicBody->setGlobalPose(physx::PxTransform(inTransform.Position.X,inTransform.Position.Y,inTransform.Position.Z));
}

void RigidbodyComponent::OverrideTransform(Transform3DComponent &inTransform) {
    DynamicBody->setGlobalPose(physx::PxTransform(inTransform.Position.X,inTransform.Position.Y,inTransform.Position.Z));
}

void physicat::entity::RigidbodyComponent::CacheDelta(physicat::math::Vector3 inDelta) {
    CachedDelta.X += inDelta.X;
    CachedDelta.Y += inDelta.Y;
    CachedDelta.Z += inDelta.Z;
}

void physicat::entity::RigidbodyComponent::AddDelta(physicat::math::Vector3 inDelta) {
    Delta.X += inDelta.X + CachedDelta.X;
    Delta.Y += inDelta.Y + CachedDelta.Y;
    Delta.Z += inDelta.Z + CachedDelta.Z;

    CachedDelta.X = 0;
    CachedDelta.Y = 0;
    CachedDelta.Z = 0;

    physicat::Log("Main Thread Delta Sync Write", TestDelta);
}

