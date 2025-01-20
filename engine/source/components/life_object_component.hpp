//
// Created by Akira Mujawar on 01/08/24.
//

#ifndef MEOWENGINE_LIFE_OBJECT_COMPONENT_HPP
#define MEOWENGINE_LIFE_OBJECT_COMPONENT_HPP

#include "pstring.hpp"

namespace MeowEngine::entity {
    struct LifeObjectComponent {
    public:
        static void Reflect();

        LifeObjectComponent(std::string name);

//        int Id;
        MeowEngine::PString Name;

        const int GetChildCount() {
            return 0;
        }
//
//        static int s_IdCounter;
//        static int s_GetNewId();
    };
}


#endif //MEOWENGINE_LIFE_OBJECT_COMPONENT_HPP
