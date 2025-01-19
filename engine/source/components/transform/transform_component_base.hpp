//
// Created by Akira Mujawar on 03/07/24.
//

#ifndef PHYSICAT_TRANSFORM_COMPONENT_BASE_HPP
#define PHYSICAT_TRANSFORM_COMPONENT_BASE_HPP

#include "component_base.hpp"
#include "glm_wrapper.hpp"

namespace MeowEngine::entity {
class TransformComponentBase : public MeowEngine::entity::ComponentBase {

    public:
        TransformComponentBase();
        virtual ~TransformComponentBase() = default;

        virtual void Update(const float& deltaTime) = 0;
    };
}


#endif //PHYSICAT_TRANSFORM_COMPONENT_BASE_HPP
