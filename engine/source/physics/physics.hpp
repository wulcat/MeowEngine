//
// Created by Akira Mujawar on 12/08/24.
//

#ifndef PHYSICAT_PHYSICS_HPP
#define PHYSICAT_PHYSICS_HPP

namespace physicat::simulator {
    struct Physics {
        virtual void Create() = 0;
        virtual void Update(float inFixedDeltaTime) = 0;
    };
}


#endif //PHYSICAT_PHYSICS_HPP
