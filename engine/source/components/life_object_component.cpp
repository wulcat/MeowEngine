//
// Created by Akira Mujawar on 01/08/24.
//

#include "life_object_component.hpp"
#include <log.hpp>
#include "entt_reflection_wrapper.hpp"

void MeowEngine::entity::LifeObjectComponent::Reflect() {
    REGISTER_PROPERTY(LifeObjectComponent, Name, MeowEngine::PString);

    MeowEngine::Log("Reflected", "LifeObjectComponent");
}

MeowEngine::entity::LifeObjectComponent::LifeObjectComponent(std::string name)
: Name(name)
//, Id(MeowEngine::entity::LifeObjectComponent::s_GetNewId())
{}

//int MeowEngine::entity::LifeObjectComponent::s_GetNewId() {
//    return s_IdCounter++;
//}

//int MeowEngine::entity::LifeObjectComponent::s_IdCounter = 0;