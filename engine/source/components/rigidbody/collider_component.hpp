//
// Created by Akira Mujawar on 30/08/24.
//

#ifndef MEOWENGINE_COLLIDER_COMPONENT_HPP
#define MEOWENGINE_COLLIDER_COMPONENT_HPP

#include <component_base.hpp>
#include <transform3d_component.hpp>
#include <collider_type.hpp>

#include <box_collider_data.hpp>

namespace MeowEngine::entity {
    class ColliderComponent : public entity::ComponentBase {

    public:
        static void Reflect();

        ColliderComponent(entity::ColliderType inType, entity::BoxColliderData* inData);
        virtual ~ColliderComponent() = default;

        physx::PxGeometry& GetGeometry();
        void SetPhysicsBody(physx::PxActor* inActor);

    private:
        entity::ColliderType Type;
        entity::ColliderData* Data;
        physx::PxActor* Body;
    };
}


#endif //MEOWENGINE_COLLIDER_COMPONENT_HPP
