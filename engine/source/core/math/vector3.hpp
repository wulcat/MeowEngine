//
// Created by Akira Mujawar on 13/07/24.
//

#ifndef PHYSICAT_VECTOR3_HPP
#define PHYSICAT_VECTOR3_HPP

namespace physicat::math {
    struct Vector3 {
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
    };
}


#endif //PHYSICAT_VECTOR3_HPP
