//
// Created by Akira Mujawar on 09/01/25.
//

#ifndef PHYSICAT_PROPERTY_TYPE_HPP
#define PHYSICAT_PROPERTY_TYPE_HPP

#include "type_traits"
#include "typeinfo"

using namespace std;

namespace MeowEngine {
    enum PropertyType {
        NOT_DEFINED,
        PRIMITIVE,
        ARRAY,
        POINTER,
        ENUM,
        CLASS_OR_STRUCT
    };

    template<typename Type>
    constexpr PropertyType GetPropertyType() {
        if constexpr (std::is_fundamental_v<Type>) {
            return PropertyType::PRIMITIVE;
        }
        else if constexpr (std::is_array_v<Type>) {
            return PropertyType::ARRAY;
        }
        else if constexpr (std::is_class_v<Type>) {
            return PropertyType::CLASS_OR_STRUCT;
        }
        else {
            return PropertyType::NOT_DEFINED;
        }
    }

    template<typename Type>
    constexpr const std::type_info& GetPropertyTypeId() {
        return typeid(Type);
    }

}

#endif //PHYSICAT_PROPERTY_TYPE_HPP
