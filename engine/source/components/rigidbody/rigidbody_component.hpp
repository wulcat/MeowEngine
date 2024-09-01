//
// Created by Akira Mujawar on 30/08/24.
//

#ifndef PHYSICAT_RIGIDBODY_COMPONENT_HPP
#define PHYSICAT_RIGIDBODY_COMPONENT_HPP

#include <component_base.hpp>
#include <transform3d_component.hpp>
#include "PxPhysicsAPI.h"

using namespace physicat::entity;

namespace physicat::entity {
    class RigidbodyComponent : public physicat::entity::ComponentBase {
    public:
        explicit RigidbodyComponent();
        virtual ~RigidbodyComponent() = default;

        /**
         * update transform using rigidbody transform
         * @param inTransform
         */
        void UpdateTransform(entity::Transform3DComponent& inTransform);

        /**
         * update rigidbody transform using transform
         * @param inTransform
         */
        void OverrideTransform(entity::Transform3DComponent& inTransform);

        void SetPhysicsBody(physx::PxRigidDynamic* inBody);

    private:
        physx::PxRigidDynamic* DynamicBody;
    };
}


#endif //PHYSICAT_RIGIDBODY_COMPONENT_HPP
