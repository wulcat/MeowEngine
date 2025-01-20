//
// Created by Akira Mujawar on 30/08/24.
//

#ifndef MEOWENGINE_BOX_COLLIDER_DATA_HPP
#define MEOWENGINE_BOX_COLLIDER_DATA_HPP


#include "transform3d_component.hpp"
#include "collider_data.hpp"

namespace MeowEngine::entity {
    class BoxColliderData : public entity::ColliderData {
    public:
        explicit BoxColliderData();
        virtual ~BoxColliderData() = default;

        physx::PxGeometry& GetGeometry() override;
    private:
        physx::PxBoxGeometry Geometry;
    };
}


#endif //MEOWENGINE_BOX_COLLIDER_DATA_HPP
