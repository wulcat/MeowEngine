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
    void *classObject = componentStorage->value(changedEntity);

    for(int i = inPropertyChange.PropertyNames.size() - 1; i > 0; i--) {
        std::string propertyName = inPropertyChange.PropertyNames[i];
        std::vector<physicat::ReflectionProperty> properties = GetProperties(propertyName);

        // remove once we have the data
        i--;

        for (const physicat::ReflectionProperty &property: properties) {
            propertyName = inPropertyChange.PropertyNames[i];
            if (property.Name == propertyName) {
                // if it was last property, we apply the data
                if(i == 0) {
                    property.Set(classObject, inPropertyChange.Data);
                }
                    // if it wasn't last property, we get the property object for next iteration
                else {
                    classObject = property.Get(classObject);
                }

                // we have found our property, move out of for loop
                break;
            }
        }
    }

}