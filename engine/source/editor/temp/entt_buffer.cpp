//
// Created by Akira Mujawar on 05/01/25.
//

#include "entt_buffer.hpp"

MeowEngine::EnttBuffer::EnttBuffer()
 : Staging() {}

entt::registry& MeowEngine::EnttBuffer::GetStaging() {
    return Staging;
}

entt::entity MeowEngine::EnttBuffer::Create() {
    entt::entity entity = Current.create();
    Final.create(entity);

    StagingCreation.enqueue(entity);

    return entity;
}
