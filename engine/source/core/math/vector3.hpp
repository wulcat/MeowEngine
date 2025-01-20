//
// Created by Akira Mujawar on 13/07/24.
//

#ifndef MEOWENGINE_VECTOR3_HPP
#define MEOWENGINE_VECTOR3_HPP

namespace MeowEngine::math {
    struct Vector3 {
        static void Reflect();

        Vector3()
        : X (0)
        , Y (0)
        , Z (0) {}

        Vector3(float x, float y, float z)
        : X (x)
        , Y (y)
        , Z (z) {}

        float X;
        float Y;
        float Z;

        // allows us to something like vec[1] = 2 instead of vec.y = 2
        float& operator[](int index) {
            switch (index) {
                case 0: return X;
                case 1: return Y;
                case 2: return Z;
                default: return X;
            }
        }

        const float& operator[](int index) const {
            switch (index) {
                case 0: return X;
                case 1: return Y;
                case 2: return Z;
                default: return X;
            }
        }

        Vector3 operator-(const Vector3& in) const {
            return {
                X - in.X,
                Y - in.Y,
                Z - in.Z
            };
        }

    };
}


#endif //MEOWENGINE_VECTOR3_HPP
