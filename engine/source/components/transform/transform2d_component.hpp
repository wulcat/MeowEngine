//
// Created by Akira Mujawar on 03/07/24.
//

#ifndef MEOWENGINE_TRANSFORM2D_COMPONENT_HPP
#define MEOWENGINE_TRANSFORM2D_COMPONENT_HPP


#include "transform_component_base.hpp"

namespace MeowEngine::entity {
    class Transform2DComponent : public MeowEngine::entity::TransformComponentBase{
    public:
        static void Reflect();
    };
}



#endif //MEOWENGINE_TRANSFORM2D_COMPONENT_HPP
