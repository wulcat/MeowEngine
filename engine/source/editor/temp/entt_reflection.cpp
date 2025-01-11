//
// Created by Akira Mujawar on 08/01/25.
//

#include "entt_reflection.hpp"

#include "log.hpp"

bool physicat::EnttReflection::HasComponent(entt::id_type inId) {
    return Components.find(inId) != Components.end();
}

bool physicat::EnttReflection::HasProperty(std::string inPropertyName) {
    return Properties.find(inPropertyName) != Properties.end();
}

std::string physicat::EnttReflection::GetComponentName(entt::id_type inId) {
    return Components[inId];
}

std::vector<physicat::ReflectionProperty> physicat::EnttReflection::GetProperties(std::string inClassName) {
    return Properties[inClassName];
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

void physicat::EnttReflection::ApplyPropertyChange(physicat::ReflectionPropertyChange& inPropertyChange, entt::registry& inRegistry) {
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
            physicat::ReflectionProperty property = inPropertyChange.ClassProperties[i];
            classObject = property.Get(classObject);

            if(i == 0) {
                ApplyPropertyChangeData(property.TypeName, inPropertyChange, classObject);
            }
        }
    }
}