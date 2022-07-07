//
// Created by Akira Lynn on 07/07/22.
//

#pragma once

#ifndef PHYSICAT_FLUID_HPP
#define PHYSICAT_FLUID_HPP

#include "grid_block.hpp"
#include "SDL.h"
#include "cstdlib"
#include "math.h"
// create blocks
// render it
// clear and render again
// give colors

namespace physicat {
    class Fluid {
    public:
        explicit Fluid(int size);

        ~Fluid();

        void AddDensity(int x, int y, int z, float amount);

        void AddVelocity(int x, int y, int z, float amountX, float amountY, float amountZ);

        void Render(SDL_Renderer *renderer);

        void Update();

    private:
        static GridBlock *CreateBlock();

        void UpdateGridBlock();

        void SetBoundary(int specialB, int paramIndex);

        void LinearSolve(int specialB, int paramIndex0, int paramIndex1, float a, float c, int iterator);

        void Diffuse(int b, int paramIndex0, int paramIndex1, float diff, int iterator);

        void Project(int vxIndex, int vyIndex, int vzIndex, int pIndex, int divIndex, int iterator);

        void Advect(int specialB, int dIndex, int d0Index, int vxIndex, int vyIndex, int vzIndex);

        GridBlock *GetBlock(int x, int y, int z);

        int size;
        int totalNumberOfBlocks;
        float dt;
        float diffusion;
        float viscosity;
        int currentAngle = 0;

        GridBlock *grid; // ((x) + (y) * N + (z) * N * N)
    };

}
#endif //PHYSICAT_FLUID_HPP
