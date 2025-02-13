//
// Created by Akira Mujawar on 12/02/25.
//

#ifndef MEOWENGINE_SPHERE_COLLIDER_DATA_HPP
#define MEOWENGINE_SPHERE_COLLIDER_DATA_HPP

#include "collider_data.hpp"

namespace MeowEngine {

    class SphereColliderData : public entity::ColliderData {
    public:
        // using explicit prevents any implicit conerstion / copy-init
        // SphereColliderData data = 0 // error
        // SphereColliderData data(); // success
        explicit SphereColliderData();
        explicit SphereColliderData(float inRadius);

        virtual ~SphereColliderData() = default;

        physx::PxGeometry& GetGeometry() override;

        float Radius;
    private:

        physx::PxSphereGeometry Geometry;
    };

} // MeowEngine

#endif //MEOWENGINE_SPHERE_COLLIDER_DATA_HPP
