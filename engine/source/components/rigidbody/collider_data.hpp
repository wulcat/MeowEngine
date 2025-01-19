//
// Created by Akira Mujawar on 01/09/24.
//

#ifndef PHYSICAT_COLLIDER_DATA_HPP
#define PHYSICAT_COLLIDER_DATA_HPP

#include "PxPhysicsAPI.h"

namespace MeowEngine::entity {
    class ColliderData {
    public:
        virtual physx::PxGeometry& GetGeometry() = 0;
    };
}


#endif //PHYSICAT_COLLIDER_DATA_HPP
