//
// Created by Akira Lynn on 07/07/22.
//

#pragma once

using namespace std;

#include <string>
// TODO: If used data locality this could be faster to iterate (let's try n implement that once simulated)
struct GridBlock {
    float s;
    float density; // strength

    float vx;
    float vy;
    float vz;

    float vxPrevious;
    float vyPrevious;
    float vzPrevious;

    float* values[8]{&s, &density, &vx, &vy, &vz, &vxPrevious, &vyPrevious, &vzPrevious};

    std::string ToString() {
        return  std::to_string(s) + ", " +
                std::to_string(density) + ", " +
                std::to_string(vx) + ", " +
                std::to_string(vy) + ", " +
                std::to_string(vz) + ", " +
                std::to_string(vxPrevious) + ", " +
                std::to_string(vyPrevious) + ", " +
                std::to_string(vzPrevious);

    }
    GridBlock() :
            s(0),
            density(0),
            vx(0),
            vy(0),
            vz(0),
            vxPrevious(0),
            vyPrevious(0),
            vzPrevious(0){

//        *values[1] = *values[0];
//        values[0] = &s;
//        values[1] = &density;
//        values[2] = &vx;
//        values[3] = &vy;
//        values[4] = &vxPrevious;
//        values[5] = &vyPrevious;
    }
};

//typedef struct GridBlock FluidCube;