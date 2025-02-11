//
// Created by Akira Mujawar on 11/02/25.
//

#ifndef MEOWENGINE_QUATERNION_HPP
#define MEOWENGINE_QUATERNION_HPP

namespace MeowEngine::Math {

    struct Quaternion {
        static void Reflect();

        float X;
        float Y;
        float Z;
        float W;
    };

} // MeowEngine

#endif //MEOWENGINE_QUATERNION_HPP
