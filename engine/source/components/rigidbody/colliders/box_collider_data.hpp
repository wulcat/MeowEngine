//
// Created by Akira Mujawar on 30/08/24.
//

#ifndef MEOWENGINE_BOX_COLLIDER_DATA_HPP
#define MEOWENGINE_BOX_COLLIDER_DATA_HPP

#include "collider_data.hpp"
#include "math_wrapper.hpp"

namespace MeowEngine::entity {
    class BoxColliderData : public entity::ColliderData {
    public:
        // using explicit prevents any implicit conversion / copy-init
        // BoxColliderData data = 0; // error
        // BoxColliderData data(); // success
        explicit BoxColliderData();
        explicit BoxColliderData(MeowEngine::math::Vector3 inSize);
        virtual ~BoxColliderData() = default;

        physx::PxGeometry& GetGeometry() override;

        MeowEngine::math::Vector3 Size; // could a different class

    private:
        physx::PxBoxGeometry Geometry;
    };
}


#endif //MEOWENGINE_BOX_COLLIDER_DATA_HPP
