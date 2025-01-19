//
// Created by Akira Mujawar on 13/07/24.
//

#include "vector3.hpp"
#include "entt_reflection_wrapper.hpp"

void MeowEngine::math::Vector3::Reflect() {
//    auto test = GET_PROPERTY_TYPE(MeowEngine::math::Vector3);
    REGISTER_PROPERTY(MeowEngine::math::Vector3, X, float);
    REGISTER_PROPERTY(MeowEngine::math::Vector3, Y, float);
    REGISTER_PROPERTY(MeowEngine::math::Vector3, Z, float);
}