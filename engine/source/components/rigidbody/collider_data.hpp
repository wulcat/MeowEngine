//
// Created by Akira Mujawar on 01/09/24.
//

#ifndef MEOWENGINE_COLLIDER_DATA_HPP
#define MEOWENGINE_COLLIDER_DATA_HPP

#include "PxPhysicsAPI.h"

namespace MeowEngine::entity {
    class ColliderData {
    public:
        virtual physx::PxGeometry& GetGeometry() = 0;
    };
}


#endif //MEOWENGINE_COLLIDER_DATA_HPP
