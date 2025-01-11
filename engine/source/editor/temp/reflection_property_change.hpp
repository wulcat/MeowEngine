//
// Created by Akira Mujawar on 11/01/25.
//

#ifndef PHYSICAT_REFLECTION_PROPERTY_CHANGE_HPP
#define PHYSICAT_REFLECTION_PROPERTY_CHANGE_HPP

#include "entt_wrapper.hpp"
#include "stack"

namespace physicat {
    class ReflectionPropertyChange {
    private:
        ReflectionPropertyChange() {}
    public:
        ReflectionPropertyChange(const std::string& inPropertyChangeName, void* inChangeData);
        ~ReflectionPropertyChange();

        int EntityId;
        entt::id_type ComponentType;
        std::stack <std::string> PropertyNames;
        void *Data;
    };
}

#endif //PHYSICAT_REFLECTION_PROPERTY_CHANGE_HPP
