//
// Created by Akira Mujawar on 08/01/25.
//

#include "entt_reflection.hpp"

bool physicat::EnttReflection::HasComponent(entt::id_type inId) {
    return Components.find(inId) != Components.end();
}

bool physicat::EnttReflection::HasProperty(std::string inPropertyName) {
    return Properties.find(inPropertyName) != Properties.end();
}

std::string physicat::EnttReflection::GetComponentName(entt::id_type inId) {
//    auto test = Components[inId];
    return Components[inId];
}

void physicat::EnttReflection::RegisterComponent(entt::id_type inId, std::string inName) {
    if(!HasComponent(inId)) {
//        Components.insert_or_assign(inId, inName);
        Components[inId] = inName;
    }
}

void physicat::EnttReflection::RegisterProperty(std::string inClassName, ReflectionProperty inProperty) {
//    if(!HasProperty(inClassName)) {
        Properties[inClassName].push_back(inProperty);
//    }
}

physicat::EnttReflection ReflectionTest();