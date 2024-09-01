//
// Created by Akira Mujawar on 12/08/24.
//

#ifndef PHYSICAT_PHYSICS_HPP
#define PHYSICAT_PHYSICS_HPP

#include <transform3d_component.hpp>
#include <rigidbody_component.hpp>
#include <collider_component.hpp>

using namespace physicat::entity;

namespace physicat::simulator {
    struct Physics {
        virtual void Create() = 0;
        virtual void Update(float inFixedDeltaTime) = 0;

        virtual void AddRigidbody(entity::Transform3DComponent& transform, entity::ColliderComponent& collider, entity::RigidbodyComponent& rigidbody) = 0;
    };
}


#endif //PHYSICAT_PHYSICS_HPP
