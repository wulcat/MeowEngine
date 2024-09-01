//
// Created by Akira Mujawar on 30/08/24.
//

#ifndef PHYSICAT_COLLIDER_COMPONENT_HPP
#define PHYSICAT_COLLIDER_COMPONENT_HPP

#include <component_base.hpp>
#include <transform3d_component.hpp>
#include <collider_type.hpp>

#include <box_collider_data.hpp>

namespace physicat::entity {
    class ColliderComponent : public entity::ComponentBase {

    public:
        ColliderComponent(entity::ColliderType inType, entity::BoxColliderData* inData);
        virtual ~ColliderComponent() = default;

        physx::PxGeometry& GetGeometry();

    private:
        entity::ColliderType Type;
        entity::ColliderData* Data;
    };
}


#endif //PHYSICAT_COLLIDER_COMPONENT_HPP
