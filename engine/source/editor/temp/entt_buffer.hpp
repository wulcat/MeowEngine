//
// Created by Akira Mujawar on 05/01/25.
//

#ifndef PHYSICAT_ENTT_BUFFER_HPP
#define PHYSICAT_ENTT_BUFFER_HPP

#include "entt_wrapper.hpp"
#include "triple_buffer.hpp"

using namespace std;

namespace physicat {
    struct EnttBuffer : public TripleBuffer<entt::registry> {
    public:
        EnttBuffer();

        entt::entity Create();

        template<typename ComponentType, typename... Args>
        void AddComponent(entt::entity& inEntity, Args &&...inArgs);

        template<typename ComponentType, typename... Args>
        void AddComponent(const entt::entity& inEntity, Args &&...inArgs);
    };

    template<typename Type, typename... Args>
    void physicat::EnttBuffer::AddComponent(entt::entity &inEntity, Args &&... inArgs) {
        Current.emplace<Type>(inEntity, std::forward<Args>(inArgs)...);
        Staging.emplace<Type>(inEntity, std::forward<Args>(inArgs)...);
        Final.emplace<Type>(inEntity, std::forward<Args>(inArgs)...);
    }

    template<typename Type, typename... Args>
    void physicat::EnttBuffer::AddComponent(const entt::entity &inEntity, Args &&... inArgs) {
        Current.emplace<Type>(inEntity, std::forward<Args>(inArgs)...);
        Staging.emplace<Type>(inEntity, std::forward<Args>(inArgs)...);
        Final.emplace<Type>(inEntity, std::forward<Args>(inArgs)...);
    }
}


#endif //PHYSICAT_ENTT_BUFFER_HPP
