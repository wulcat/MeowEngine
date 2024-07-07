//
// Created by Akira Mujawar on 23/06/24.
//

#include "life_object.hpp"

using physicat::core::LifeObject;

LifeObject::LifeObject()
    : TransformComponent(nullptr)
    , RenderComponent(nullptr)
    , PhysicsComponent(nullptr) {}


void LifeObject::SetTransformComponent(component::TransformComponentBase *component) {
    TransformComponent = component;
}


void LifeObject::SetRenderComponent(component::RenderComponentBase *component) {
    RenderComponent = component;
}