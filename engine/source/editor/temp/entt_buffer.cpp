//
// Created by Akira Mujawar on 05/01/25.
//

#include "entt_buffer.hpp"

physicat::EnttBuffer::EnttBuffer()
 : Staging() {}

entt::registry& physicat::EnttBuffer::GetStaging() {
    return Staging;
}

entt::entity physicat::EnttBuffer::Create() {
    entt::entity entity = Current.create();
    Final.create(entity);

    StagingCreation.enqueue(entity);

    return entity;
}
