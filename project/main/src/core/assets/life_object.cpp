//
// Created by Akira Mujawar on 23/06/24.
//

#include "life_object.hpp"

using physicat::core::LifeObject;

LifeObject::LifeObject() {}

void LifeObject::SetRenderComponent(component::RenderComponentBase *Component) {
    RenderComponent = Component;
}