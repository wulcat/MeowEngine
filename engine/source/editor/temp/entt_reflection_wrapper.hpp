//
// Created by Akira Mujawar on 08/01/25.
//
//#pragma once

#ifndef PHYSICAT_ENTT_REFLECTION_WRAPPER_HPP
#define PHYSICAT_ENTT_REFLECTION_WRAPPER_HPP

#include "entt_reflection.hpp"

namespace MeowEngine {
    extern MeowEngine::EnttReflection Reflection;

    // SPINAE principle :- substitution failure is not a error
    #define REFLECT(Type) MeowEngine::Reflection.Reflect<Type>()

    #define REGISTER_ENTT_COMPONENT(Component) \
        \
        MeowEngine::Reflection.RegisterComponent(\
            entt::type_hash<Component>().value(), \
            #Component\
        );\
        \
        REFLECT(Component);

    #define REGISTER_PROPERTY(Class, Property, Type)\
        MeowEngine::Reflection.RegisterProperty(\
            #Class,\
            {\
                #Property,                          \
                GetPropertyType<Type>(),                                    \
                GetPropertyTypeId<Type>(),          \
                #Type,                                    \
                [](void* obj, const void* value) { ((Class*)obj)->Property = *(Type*)value; },\
                [](void* obj) -> void* { return &(((Class*)obj)->Property);}\
            }\
        );\
        \
        REFLECT(Type);

}
#endif //PHYSICAT_ENTT_REFLECTION_WRAPPER_HPP
