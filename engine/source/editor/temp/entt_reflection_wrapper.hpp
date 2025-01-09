//
// Created by Akira Mujawar on 08/01/25.
//
//#pragma once

#ifndef PHYSICAT_ENTT_REFLECTION_WRAPPER_HPP
#define PHYSICAT_ENTT_REFLECTION_WRAPPER_HPP

#include "entt_reflection.hpp"

//namespace {
    extern physicat::EnttReflection ReflectionTest;
//}
//physicat::EnttReflection ReflectionTest;

namespace physicat {
    // SPINAE principle :- substitution failure is not a error
    #define REFLECT(Type) ReflectionTest.Reflect<Type>()

    #define REGISTER_ENTT_COMPONENT(Component) \
        \
        ReflectionTest.RegisterComponent(\
            entt::type_hash<Component>().value(), \
            #Component\
        );\
        \
        REFLECT(Component);

    #define REGISTER_PROPERTY(Class, Property, Type)\
        ReflectionTest.RegisterProperty(\
            #Class,\
            {\
                #Property,\
                [](void* obj, const void* value) { ((Class*)obj)->Property = *(Type*)value; },\
                [](void* obj) -> void* { return &(((Class*)obj)->Property);}\
            }\
        );\
        \
        REFLECT(Type);

}
#endif //PHYSICAT_ENTT_REFLECTION_WRAPPER_HPP
