//
// Created by Akira Mujawar on 27/01/25.
//

#ifndef MEOWENGINE_ENTT_SINGLE_BUFFER_HPP
#define MEOWENGINE_ENTT_SINGLE_BUFFER_HPP

#include "entt_buffer_base.hpp"

namespace MeowEngine {

    struct EnttSingleBuffer : public MeowEngine::EnttBufferBase {
        EnttSingleBuffer();

        entt::registry& GetCurrent() override;
        entt::entity AddEntity() override;

        template<typename ComponentType, typename... Args>
        void AddComponent(entt::entity& inEntity, Args &&...inArgs);

        template<typename ComponentType, typename... Args>
        void AddComponent(const entt::entity& inEntity, Args &&...inArgs);

        void ApplyPropertyChange() override;

    protected:
        entt::registry Current;
    };

    template<typename ComponentType, typename... Args>
    void MeowEngine::EnttSingleBuffer::AddComponent(entt::entity &inEntity, Args &&... inArgs) {
        Current.emplace<ComponentType>(inEntity, std::forward<Args>(inArgs)...);
    }

    template<typename ComponentType, typename... Args>
    void MeowEngine::EnttSingleBuffer::AddComponent(const entt::entity &inEntity, Args &&... inArgs) {
        Current.emplace<ComponentType>(inEntity, std::forward<Args>(inArgs)...);
    }

} // MeowEngine

#endif //MEOWENGINE_ENTT_SINGLE_BUFFER_HPP
