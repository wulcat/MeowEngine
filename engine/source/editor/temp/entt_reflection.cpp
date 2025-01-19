//
// Created by Akira Mujawar on 08/01/25.
//

#include "entt_reflection.hpp"

#include "log.hpp"

bool MeowEngine::EnttReflection::HasComponent(entt::id_type inId) {
    return Components.find(inId) != Components.end();
}

bool MeowEngine::EnttReflection::HasProperty(std::string inPropertyName) {
    return Properties.find(inPropertyName) != Properties.end();
}

std::string MeowEngine::EnttReflection::GetComponentName(entt::id_type inId) {
    return Components[inId];
}

std::vector<MeowEngine::ReflectionProperty> MeowEngine::EnttReflection::GetProperties(std::string inClassName) {
    return Properties[inClassName];
}

void MeowEngine::EnttReflection::RegisterComponent(entt::id_type inId, std::string inName) {
    if(!HasComponent(inId)) {
//        Components.insert_or_assign(inId, inName);
        Components[inId] = inName;
    }
}

void MeowEngine::EnttReflection::RegisterProperty(std::string inClassName, ReflectionProperty inProperty) {
//    if(!HasProperty(inClassName)) {
        Properties[inClassName].push_back(inProperty);
//    }
}

void MeowEngine::EnttReflection::ApplyPropertyChange(MeowEngine::ReflectionPropertyChange& inPropertyChange, entt::registry& inRegistry) {
    auto changedEntity = static_cast<entt::entity>(inPropertyChange.EntityId);

    entt::basic_registry<>::common_type *componentStorage = inRegistry.storage(inPropertyChange.ComponentType);
    std::string componentName = GetComponentName(inPropertyChange.ComponentType);

    void *classObject = componentStorage->value(changedEntity);

    // if component has direct changes
    if(inPropertyChange.ClassProperties.empty()) {
        ApplyPropertyChangeData(componentName, inPropertyChange, classObject);
    }
    // if classes / struct within component has changes
    else {
        for(int i = inPropertyChange.ClassProperties.size() - 1; i >= 0; i--) {
            MeowEngine::ReflectionProperty property = inPropertyChange.ClassProperties[i];
            classObject = property.Get(classObject);

            if(i == 0) {
                ApplyPropertyChangeData(property.TypeName, inPropertyChange, classObject);
            }
        }
    }
}