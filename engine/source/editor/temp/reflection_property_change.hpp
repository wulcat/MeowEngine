//
// Created by Akira Mujawar on 11/01/25.
//

#ifndef PHYSICAT_REFLECTION_PROPERTY_CHANGE_HPP
#define PHYSICAT_REFLECTION_PROPERTY_CHANGE_HPP

#include "entt_wrapper.hpp"

namespace physicat {
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
        std::vector <std::string> PropertyNames;
        void* Data;
        std::function<void(void*)> DataDeleter;
    };
}

#endif //PHYSICAT_REFLECTION_PROPERTY_CHANGE_HPP
