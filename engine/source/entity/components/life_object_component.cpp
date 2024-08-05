//
// Created by Akira Mujawar on 01/08/24.
//

#include "life_object_component.hpp"

physicat::entity::LifeObjectComponent::LifeObjectComponent(std::string name)
: Name(name)
, Id(physicat::entity::LifeObjectComponent::s_GetNewId())
{}

int physicat::entity::LifeObjectComponent::s_GetNewId() {
    return s_IdCounter++;
}

int physicat::entity::LifeObjectComponent::s_IdCounter = 0;