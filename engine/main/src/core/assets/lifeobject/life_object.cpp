//
// Created by Akira Mujawar on 23/06/24.
//

#include "life_object.hpp"

using physicat::core::LifeObject;

LifeObject::LifeObject(std::string name)
    : Name(name)
    , TransformComponent(nullptr)
    , RenderComponent(nullptr)
    , PhysicsComponent(nullptr)
    , Id(physicat::core::LifeObject::s_GetNewId()){}


void LifeObject::SetTransformComponent(component::TransformComponentBase *component) {
    TransformComponent = component;
}


void LifeObject::SetRenderComponent(component::RenderComponentBase *component) {
    RenderComponent = component;
}

int LifeObject::s_GetNewId() {
    s_IdCounter += 1;
    return s_IdCounter;
}

int LifeObject::s_IdCounter = 0;