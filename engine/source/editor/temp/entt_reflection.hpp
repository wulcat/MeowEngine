//
// Created by Akira Mujawar on 08/01/25.
//

#ifndef PHYSICAT_ENTT_REFLECTION_HPP
#define PHYSICAT_ENTT_REFLECTION_HPP

#include "unordered_map"
#include "entt_wrapper.hpp"
#include "vector"
#include "reflection_property.hpp"
#include "reflection_property_change.hpp"
#include "string"
#include "log.hpp"
using namespace std;

namespace physicat {
    class EnttReflection {

    public:
        EnttReflection() {
            physicat::Log("Reflection", "Constructed");
        }
        ~EnttReflection() {
            physicat::Log("Reflection", "Destructed");
        }

        bool HasComponent(entt::id_type inId);
        bool HasProperty(std::string inPropertyName);
        std::string GetComponentName(entt::id_type inId);
        std::vector<ReflectionProperty> GetProperties(std::string inClassName);

        template<typename Type>
        void Reflect();

        void RegisterComponent(entt::id_type inId,  std::string inName);
        void RegisterProperty(std::string inClassName, ReflectionProperty inProperty);

        void ApplyPropertyChange(physicat::ReflectionPropertyChange& inPropertyChange, entt::registry& inRegistry);

        void ApplyPropertyChangeData(std::string& inClassName, physicat::ReflectionPropertyChange& inPropertyChange, void* inClassObject) {
            std::vector<physicat::ReflectionProperty> properties = GetProperties(inClassName);

            for(const physicat::ReflectionProperty &property : properties) {
                if(property.Name == inPropertyChange.PropertyName) {
                    property.Set(inClassObject, inPropertyChange.Data);
                }
            }
        }

    private:
        std::unordered_map<entt::id_type, std::string> Components;
        std::unordered_map<std::string, std::vector<ReflectionProperty>> Properties;
    };

    template<typename Type>
    void EnttReflection::Reflect() {
        if constexpr (std::is_fundamental_v<Type>) {
//            return PropertyType::PRIMITIVE;
        }
        else if constexpr (std::is_array_v<Type>) {
//            return PropertyType::ARRAY;
        }
        else if constexpr (std::is_class_v<Type>) {
            Type::Reflect();
        }
        else {
//            return PropertyType::NOT_DEFINED;
        }
    }
}


#endif //PHYSICAT_ENTT_REFLECTION_HPP
