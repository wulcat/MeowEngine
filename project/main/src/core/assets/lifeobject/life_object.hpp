//
// Created by Akira Mujawar on 23/06/24.
//

#ifndef PHYSICAT_LIFE_OBJECT_HPP
#define PHYSICAT_LIFE_OBJECT_HPP


#include "render_component_base.hpp"
#include "physics_component_base.hpp"
#include "transform_component_base.hpp"

// NOTE: we are linking all the files using cmake glob which isn't optimal.
// Once we have good structured layout for our engine, we will hard link using paths
//#include "src/core/assets/components/renders/render_component_base.hpp"
//#include "src/core/assets/components/physics/physics_component_base.hpp"

namespace physicat::core {
    // NOTE: for now we will go with hard created components.
    // Eventually we will use ECS for this.
    // ecs will be a deeper topic and will come to it later on.
    // ecs + hirachey (bubble effect)
    class LifeObject {
    public:
        LifeObject();

        void SetTransformComponent(component::TransformComponentBase* component);
        void SetRenderComponent(component::RenderComponentBase* component);

//    private:
// NOTE: Checkout different ways of pointers to use here along with internal pointer we have created
        component::TransformComponentBase* TransformComponent;
        component::RenderComponentBase* RenderComponent;
        component::PhysicsComponentBase* PhysicsComponent;
    };
}


#endif //PHYSICAT_LIFE_OBJECT_HPP
