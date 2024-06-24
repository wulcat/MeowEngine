//
// Created by Akira Mujawar on 23/06/24.
//

#ifndef PHYSICAT_LIFE_OBJECT_HPP
#define PHYSICAT_LIFE_OBJECT_HPP


#include "src/core/assets/components/renders/render_component_base.hpp"
#include "src/core/assets/components/physics/physics_component_base.hpp"

namespace physicat::core {
    class LifeObject {
    public:
        LifeObject();

        void SetRenderComponent(component::RenderComponentBase* Component);

//    private:
        component::RenderComponentBase* RenderComponent;
        component::PhysicsComponentBase* PhysicsComponent;
    };
}


#endif //PHYSICAT_LIFE_OBJECT_HPP
