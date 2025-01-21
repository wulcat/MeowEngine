//
// Created by Akira Mujawar on 12/08/24.
//

#ifndef MEOWENGINE_PHYSICS_SYSTEM_HPP
#define MEOWENGINE_PHYSICS_SYSTEM_HPP

#include <transform3d_component.hpp>
#include <rigidbody_component.hpp>
#include <collider_component.hpp>

using namespace MeowEngine::entity;

namespace MeowEngine::simulator {
    struct PhysicsSystem {
        virtual void Create() = 0;
        virtual void Update(float inFixedDeltaTime) = 0;

        virtual void AddRigidbody(entity::Transform3DComponent& transform, entity::ColliderComponent& collider, entity::RigidbodyComponent& rigidbody) = 0;
    };
}


#endif //MEOWENGINE_PHYSICS_SYSTEM_HPP
