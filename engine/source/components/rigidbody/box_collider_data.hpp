//
// Created by Akira Mujawar on 30/08/24.
//

#ifndef PHYSICAT_BOX_COLLIDER_DATA_HPP
#define PHYSICAT_BOX_COLLIDER_DATA_HPP


#include "transform3d_component.hpp"
#include "collider_data.hpp"

namespace physicat::entity {
    class BoxColliderData : public entity::ColliderData {
    public:
        explicit BoxColliderData(const entity::Transform3DComponent* inTransform);
        virtual ~BoxColliderData() = default;

        physx::PxGeometry& GetGeometry() override;
    private:
        physx::PxBoxGeometry Geometry;
    };
}


#endif //PHYSICAT_BOX_COLLIDER_DATA_HPP
