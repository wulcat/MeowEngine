//
// Created by Akira Mujawar on 01/08/24.
//

#ifndef PHYSICAT_LIFE_OBJECT_COMPONENT_HPP
#define PHYSICAT_LIFE_OBJECT_COMPONENT_HPP

#include "pstring.hpp"

namespace physicat::entity {
    struct LifeObjectComponent {
    public:
        static void Reflect();

        LifeObjectComponent(std::string name);

//        int Id;
        physicat::PString Name;

        const int GetChildCount() {
            return 0;
        }
//
//        static int s_IdCounter;
//        static int s_GetNewId();
    };
}


#endif //PHYSICAT_LIFE_OBJECT_COMPONENT_HPP
