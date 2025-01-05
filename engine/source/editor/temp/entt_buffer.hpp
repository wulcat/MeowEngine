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
    };
}


#endif //PHYSICAT_ENTT_BUFFER_HPP
