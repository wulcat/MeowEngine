//
// Created by Akira Mujawar on 03/07/24.
//

#ifndef PHYSICAT_TRANSFORM2D_COMPONENT_HPP
#define PHYSICAT_TRANSFORM2D_COMPONENT_HPP


#include "transform_component_base.hpp"

namespace MeowEngine::entity {
    class Transform2DComponent : public MeowEngine::entity::TransformComponentBase{
    public:
        static void Reflect();
    };
}



#endif //PHYSICAT_TRANSFORM2D_COMPONENT_HPP
