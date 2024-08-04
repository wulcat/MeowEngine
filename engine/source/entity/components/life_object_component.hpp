//
// Created by Akira Mujawar on 01/08/24.
//

#ifndef PHYSICAT_LIFE_OBJECT_COMPONENT_HPP
#define PHYSICAT_LIFE_OBJECT_COMPONENT_HPP

#include "string"

namespace physicat::entity {
    struct LifeObjectComponent {
    public:
        LifeObjectComponent(std::string name)
        {
            Name = name;
        }
        int Id;
        std::string Name;

        const int GetChildCount() {
            return 0;
        }
    };
}


#endif //PHYSICAT_LIFE_OBJECT_COMPONENT_HPP
