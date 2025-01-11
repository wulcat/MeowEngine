//
// Created by Akira Mujawar on 11/01/25.
//

#include "reflection_property_change.hpp"

#include <utility>
#include "log.hpp"

physicat::ReflectionPropertyChange::ReflectionPropertyChange(const std::string& inPropertyChangeName, void* inChangeData, std::function<void(void*)> inDataDeleter)
        : PropertyNames()
        , Data(inChangeData)
        , DataDeleter(std::move(inDataDeleter)){
    physicat::Log("physicat::ReflectionPropertyChange", "Constructed");
    PropertyNames.push_back(inPropertyChangeName);
}

physicat::ReflectionPropertyChange::~ReflectionPropertyChange() {
    physicat::Log("physicat::ReflectionPropertyChange", "Destructed");

//    physicat::Log("Before Des", *static_cast<int*>(Data));
    DataDeleter(Data);
//    delete static_cast<int*>(Data);
//    physicat::Log("After Des", *static_cast<int*>(Data));
}