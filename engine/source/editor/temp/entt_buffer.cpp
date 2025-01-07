//
// Created by Akira Mujawar on 05/01/25.
//

#include "entt_buffer.hpp"

//template<typename BufferType>
physicat::EnttBuffer::EnttBuffer() {}

entt::entity physicat::EnttBuffer::Create() {
    entt::entity entity = Current.create();
    Staging.create();
    Final.create();

    return entity;
}
