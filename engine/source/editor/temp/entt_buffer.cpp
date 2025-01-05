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

template<typename Type, typename... Args>
void physicat::EnttBuffer::AddComponent(entt::entity &inEntity, Args &&... inArgs) {
    Current.emplace<Type>(inEntity, std::forward<Args>(inArgs)...);
    Staging.emplace<Type>(inEntity, std::forward<Args>(inArgs)...);
    Final.emplace<Type>(inEntity, std::forward<Args>(inArgs)...);
}
