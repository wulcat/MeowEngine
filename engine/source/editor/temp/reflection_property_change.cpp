//
// Created by Akira Mujawar on 11/01/25.
//

#include "reflection_property_change.hpp"
#include "log.hpp"

physicat::ReflectionPropertyChange::ReflectionPropertyChange(const std::string& inPropertyChangeName, void* inChangeData)
        : PropertyNames()
        , Data(inChangeData) {
    physicat::Log("physicat::ReflectionPropertyChange", "Constructed");
    PropertyNames.push(inPropertyChangeName);
}

physicat::ReflectionPropertyChange::~ReflectionPropertyChange() {
    physicat::Log("physicat::ReflectionPropertyChange", "Destructed");
}