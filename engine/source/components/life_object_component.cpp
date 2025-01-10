//
// Created by Akira Mujawar on 01/08/24.
//

#include "life_object_component.hpp"
#include <log.hpp>
#include "entt_reflection_wrapper.hpp"

void physicat::entity::LifeObjectComponent::Reflect() {
    REGISTER_PROPERTY(LifeObjectComponent, Name, physicat::PString);

    physicat::Log("Reflected", "LifeObjectComponent");
}

physicat::entity::LifeObjectComponent::LifeObjectComponent(std::string name)
: Name(name)
//, Id(physicat::entity::LifeObjectComponent::s_GetNewId())
{}

//int physicat::entity::LifeObjectComponent::s_GetNewId() {
//    return s_IdCounter++;
//}

//int physicat::entity::LifeObjectComponent::s_IdCounter = 0;