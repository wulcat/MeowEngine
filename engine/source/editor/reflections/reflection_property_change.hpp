//
// Created by Akira Mujawar on 11/01/25.
//

#ifndef MEOWENGINE_REFLECTION_PROPERTY_CHANGE_HPP
#define MEOWENGINE_REFLECTION_PROPERTY_CHANGE_HPP

#include "entt_wrapper.hpp"
#include "reflection_property.hpp"

namespace MeowEngine {
    class ReflectionPropertyChange {
    private:
        ReflectionPropertyChange() {}
    public:
        ReflectionPropertyChange(const std::string& inPropertyChangeName, void* inChangeData, std::function<void(void*)> inDataDeleter);
        ~ReflectionPropertyChange();

        static void Assign(ReflectionPropertyChange*& inTarget, ReflectionPropertyChange* inValue) {
            if(inValue != nullptr) {
                inTarget = inValue;
            }
        }

        int EntityId;
        entt::id_type ComponentType;
        std::string PropertyName;
        std::vector <MeowEngine::ReflectionProperty> ClassProperties;
        void* Data;
        std::function<void(void*)> DataDeleter;
    };
}

#endif //MEOWENGINE_REFLECTION_PROPERTY_CHANGE_HPP
