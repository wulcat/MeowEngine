//
// Created by Akira Mujawar on 11/01/25.
//

#include "reflection_property_change.hpp"

#include <utility>
#include "log.hpp"

MeowEngine::ReflectionPropertyChange::ReflectionPropertyChange(const std::string& inPropertyChangeName, void* inChangeData, std::function<void(void*)> inDataDeleter)
        : PropertyName(inPropertyChangeName)
        , ClassProperties()
        , Data(inChangeData)
        , DataDeleter(std::move(inDataDeleter)){
    MeowEngine::Log("MeowEngine::ReflectionPropertyChange", "Constructed");
}

MeowEngine::ReflectionPropertyChange::~ReflectionPropertyChange() {
    MeowEngine::Log("MeowEngine::ReflectionPropertyChange", "Destructed");

//    MeowEngine::Log("Before Des", *static_cast<int*>(Data));
    DataDeleter(Data);
//    delete static_cast<int*>(Data);
//    MeowEngine::Log("After Des", *static_cast<int*>(Data));
}