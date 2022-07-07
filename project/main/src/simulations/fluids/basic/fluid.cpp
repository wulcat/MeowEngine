//
// Created by Akira Lynn on 07/07/22.
//

#include "fluid.hpp"
#include "../../../core/log.hpp"

using namespace physicat;
//using physicat::Fluid;


Fluid::Fluid(int size) :
        size(size),
        totalNumberOfBlocks(size * size * size),
        dt(0.1f),
        diffusion(0),
        viscosity(0.00001f) {

    grid = new GridBlock[totalNumberOfBlocks];

    // Create empty blocks in our grid in our fluid container
    for(int i = 0 ; i < totalNumberOfBlocks ; i++) {
        grid[i] = *CreateBlock();
    }

    physicat::log("CreateBlock", std::to_string(totalNumberOfBlocks));
}

Fluid::~Fluid() {
    for(int i = 0 ; i < totalNumberOfBlocks ; i++) {
        delete &grid[i];
    }
}

GridBlock* Fluid::GetBlock(int x, int y, int z) {
    return &grid[x + y * size + z * size * size];
}

GridBlock* Fluid::CreateBlock() {
    auto gridBlock = new GridBlock() ;
    gridBlock->density = 0;

    return gridBlock;
}

void Fluid::AddDensity(int x, int y, int z, float amount) {
    GetBlock(x,y,z)->density += amount;
}

void Fluid::AddVelocity(int x, int y, int z, float amountX, float amountY, float amountZ) {
    GridBlock* block = GetBlock(x,y,z);

    block->vx += amountX;
    block->vy += amountY;
    block->vz += amountZ;
}

void Fluid::Update() {
//    int currentAngle = 0;
//    int radius = 8;
//    currentAngle += 360 / (radius * 8);
//    if(currentAngle >= 360) {
//        currentAngle = 0;
//    }
//
//    int x = size/2 + cos(currentAngle) * radius;
//    int y = size/2 + sin(currentAngle) * radius;
//
//    AddDensity(x, y, 0, 1);
//    AddVelocity(x, y,0, 1, 0,0);
    AddDensity(size/2, size/2, 0, 1);
    AddVelocity(size/2, size/2,0, 2, 0,0);
//    physicat::log("Density", std::to_string(GetBlock(size/2,size/2,0)->density));
    physicat::log("Density of middle block" , GetBlock(size/2,size/2,0)->ToString());
    physicat::log("Density of middle block - back" , GetBlock(size/2 - 1,size/2,0)->ToString());
    UpdateGridBlock();
}

void Fluid::Render(SDL_Renderer* renderer) {
    int scale = 15;
//    int random_variable = std::rand();
//    physicat::log("fluid strength: ", std::to_string(GetBlock(0,0,0)->density));
    for (int i = 1; i < size - 1; i++) {
        for (int j = 1; j < size - 1; j++) { // skip the z

            SDL_Rect rect;
            rect.x = i * scale;
            rect.y = j * scale;
            rect.w = scale;
            rect.h = scale;

            float strength = GetBlock(i, j, 0)->density;

            SDL_SetRenderDrawColor(renderer, 255 * strength, 0, 0, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void Fluid::UpdateGridBlock() {
    Diffuse(1, 5, 2, viscosity, 4);
    Diffuse(2, 6, 3, viscosity, 4);
    Diffuse(3, 7, 4, viscosity, 4);

    Project(5, 6, 7, 2, 3, 4);

    Advect(1, 2, 5, 5, 6, 7);
    Advect(2, 3, 6, 5, 6, 7);
    Advect(3, 4, 7, 5, 6, 7);

    Project(2, 3, 4, 5, 6, 4);

    Diffuse(0, 0, 1, diffusion, 4);
    Advect(0, 1, 0, 2, 3, 4);
}

void Fluid::SetBoundary(int specialB, int paramIndex) {

    for(int j = 1; j < size - 1; j++) {
        for(int i = 1; i < size - 1; i++) {
            *GetBlock(i, j, 0 )->values[paramIndex] = specialB == 3 ? -*GetBlock(i, j, 1)->values[paramIndex] : *GetBlock(i, j, 1)->values[paramIndex];
            *GetBlock(i, j, size - 1)->values[paramIndex] = specialB == 3 ? -*GetBlock(i, j, size - 2)->values[paramIndex] : *GetBlock(i, j, size - 2)->values[paramIndex];
        }
    }
    for(int k = 1; k < size - 1; k++) {
        for(int i = 1; i < size - 1; i++) {
            *GetBlock(i, 0  , k)->values[paramIndex] = specialB == 2 ? -*GetBlock(i, 1, k)->values[paramIndex] : *GetBlock(i, 1, k)->values[paramIndex];
            *GetBlock(i, size - 1, k)->values[paramIndex] = specialB == 2 ? -*GetBlock(i, size - 2, k)->values[paramIndex] : *GetBlock(i, size - 2, k)->values[paramIndex];
        }
    }
    for(int k = 1; k < size - 1; k++) {
        for(int j = 1; j < size - 1; j++) {
            *GetBlock(0  , j, k)->values[paramIndex] = specialB == 1 ? -*GetBlock(1, j, k)->values[paramIndex] : *GetBlock(1, j, k)->values[paramIndex];
            *GetBlock(size - 1, j, k)->values[paramIndex] = specialB == 1 ? -*GetBlock(size - 2, j, k)->values[paramIndex] : *GetBlock(size - 2, j, k)->values[paramIndex];
        }
    }

    *GetBlock(0, 0, 0)->values[paramIndex]      = 0.33f * (*GetBlock(1, 0, 0)->values[paramIndex]
                                       + *GetBlock(0, 1, 0)->values[paramIndex]
                                       + *GetBlock(0, 0, 1)->values[paramIndex]);
    *GetBlock(0, size - 1, 0)->values[paramIndex]     = 0.33f * (*GetBlock(1, size - 1, 0)->values[paramIndex]
                                            + *GetBlock(0, size - 2, 0)->values[paramIndex]
                                            + *GetBlock(0, size - 1, 1)->values[paramIndex]);
    *GetBlock(0, 0, size - 1)->values[paramIndex]     = 0.33f * (*GetBlock(1, 0, size - 1)->values[paramIndex]// => issue
                                            + *GetBlock(0, 1, size - 1)->values[paramIndex]
                                            + *GetBlock(0, 0, size - 2)->values[paramIndex]);
    *GetBlock(0, size - 1, size - 1)->values[paramIndex]   = 0.33f * (*GetBlock(1, size - 1, size - 1)->values[paramIndex]
                                                 + *GetBlock(0, size - 2, size - 1)->values[paramIndex]
                                                 + *GetBlock(0, size - 1, size - 2)->values[paramIndex]);
    *GetBlock(size - 1, 0, 0)->values[paramIndex]     = 0.33f * (*GetBlock(size - 2, 0, 0)->values[paramIndex]
                                            + *GetBlock(size - 1, 1, 0)->values[paramIndex]
                                            + *GetBlock(size - 1, 0, 1)->values[paramIndex]);
    *GetBlock(size - 1, size - 1, 0)->values[paramIndex]   = 0.33f * (*GetBlock(size - 2, size - 1, 0)->values[paramIndex]
                                                 + *GetBlock(size - 1, size - 2, 0)->values[paramIndex]
                                                 + *GetBlock(size - 1, size - 1, 1)->values[paramIndex]);
    *GetBlock(size - 1, 0, size - 1)->values[paramIndex]   = 0.33f * (*GetBlock(size - 2, 0, size - 1)->values[paramIndex]
                                                 + *GetBlock(size - 1, 1, size - 1)->values[paramIndex]
                                                 + *GetBlock(size - 1, 0, size - 2)->values[paramIndex]);
    *GetBlock(size - 1, size - 1, size - 1)->values[paramIndex] = 0.33f * (*GetBlock(size - 2, size - 1, size - 1)->values[paramIndex]
                                                      + *GetBlock(size - 1, size - 2, size - 1)->values[paramIndex]
                                                      + *GetBlock(size - 1, size - 1, size - 2)->values[paramIndex]);
}

void Fluid::LinearSolve(int specialB, int paramIndex0, int paramIndex1, float a, float c, int iterator) {
    float cRecip = 1.0f / c;
    for (int k = 0; k < iterator; k++) {
        for (int m = 1; m < size - 1; m++) {
            for (int j = 1; j < size - 1; j++) {
                for (int i = 1; i < size - 1; i++) {
                    *GetBlock(i, j, m)->values[paramIndex0] =
                            (*GetBlock(i, j, m)->values[paramIndex1]
                             + a*(*GetBlock(i + 1, j, m)->values[paramIndex0]
                                  + *GetBlock(i - 1, j, m)->values[paramIndex0]
                                  + *GetBlock(i, j + 1, m)->values[paramIndex0]
                                  + *GetBlock(i, j - 1, m)->values[paramIndex0]
                                  + *GetBlock(i, j, m + 1)->values[paramIndex0]
                                  + *GetBlock(i, j, m - 1)->values[paramIndex0]
                            )) * cRecip;
                }
            }
        }
        SetBoundary(specialB, paramIndex0);
    }
}

void Fluid::Diffuse(int b, int paramIndex0, int paramIndex1, float diff, int iterator) {
    float a = dt * diff * (size - 2) * (size - 2);
    LinearSolve(b, paramIndex0, paramIndex1, a, 1 + 6 * a, iterator);
}

void Fluid::Project(int vxIndex, int vyIndex, int vzIndex, int pIndex, int divIndex, int iterator) {
    for (int k = 1; k < size - 1; k++) {
        for (int j = 1; j < size - 1; j++) {
            for (int i = 1; i < size - 1; i++) {
                *GetBlock(i, j, k)->values[divIndex] = -0.5f * (
                                                                       *GetBlock(i + 1, j, k)->values[vxIndex]
                                                                - *GetBlock(i - 1, j, k)->values[vxIndex]
                                                                + *GetBlock(i, j + 1, k)->values[vyIndex]
                                                                - *GetBlock(i, j - 1, k)->values[vyIndex]
                                                                + *GetBlock(i, j, k + 1)->values[vzIndex]
                                                                - *GetBlock(i, j, k - 1)->values[vzIndex]
                ) / size;
                *GetBlock(i, j, k)->values[pIndex] = 0;
            }
        }
    }
    SetBoundary(0, divIndex);
    SetBoundary(0, pIndex);
    LinearSolve(0, pIndex, divIndex, 1, 6, iterator);

    for (int k = 1; k < size - 1; k++) {
        for (int j = 1; j < size - 1; j++) {
            for (int i = 1; i < size - 1; i++) {
                *GetBlock(i, j, k)->values[vxIndex] -= 0.5f * (*GetBlock(i + 1, j, k)->values[pIndex]
                                                              - *GetBlock(i - 1, j, k)->values[pIndex]) * size;
                *GetBlock(i, j, k)->values[vyIndex] -= 0.5f * (*GetBlock(i, j + 1, k)->values[pIndex]
                                                              - *GetBlock(i, j - 1, k)->values[pIndex]) * size;
                *GetBlock(i, j, k)->values[vzIndex] -= 0.5f * (*GetBlock(i, j, k + 1)->values[pIndex]
                                                              - *GetBlock(i, j, k - 1)->values[pIndex]) * size;
            }
        }
    }
    SetBoundary(1, vxIndex);
    SetBoundary(2, vyIndex);
    SetBoundary(3, vzIndex);
}

void Fluid::Advect(int specialB, int dIndex, int d0Index, int vxIndex, int vyIndex, int vzIndex) {
    float i0, i1, j0, j1, k0, k1;

    float dtx = dt * (size - 2);
    float dty = dt * (size - 2);
    float dtz = dt * (size - 2);

    float s0, s1, t0, t1, u0, u1;
    float tmp1, tmp2, tmp3, x, y, z;

    float Nfloat = size;
    float ifloat, jfloat, kfloat;
    int i, j, k;

    for(k = 1, kfloat = 1; k < size - 1; k++, kfloat++) {
        for(j = 1, jfloat = 1; j < size - 1; j++, jfloat++) {
            for(i = 1, ifloat = 1; i < size - 1; i++, ifloat++) {
                tmp1 = dtx * *GetBlock(i, j, k)->values[vxIndex];
                tmp2 = dty * *GetBlock(i, j, k)->values[vyIndex];
                tmp3 = dtz * *GetBlock(i, j, k)->values[vzIndex];
                x    = ifloat - tmp1;
                y    = jfloat - tmp2;
                z    = kfloat - tmp3;

                if(x < 0.5f) x = 0.5f;
                if(x > Nfloat + 0.5f) x = Nfloat + 0.5f;
                i0 = floorf(x);
                i1 = i0 + 1.0f;
                if(y < 0.5f) y = 0.5f;
                if(y > Nfloat + 0.5f) y = Nfloat + 0.5f;
                j0 = floorf(y);
                j1 = j0 + 1.0f;
                if(z < 0.5f) z = 0.5f;
                if(z > Nfloat + 0.5f) z = Nfloat + 0.5f;
                k0 = floorf(z);
                k1 = k0 + 1.0f;

                s1 = x - i0;
                s0 = 1.0f - s1;
                t1 = y - j0;
                t0 = 1.0f - t1;
                u1 = z - k0;
                u0 = 1.0f - u1;

                int i0i = i0;
                int i1i = i1;
                int j0i = j0;
                int j1i = j1;
                int k0i = k0;
                int k1i = k1;

                *GetBlock(i, j, k)->values[dIndex] =

                        s0 * ( t0 * (u0 * *GetBlock(i0i, j0i, k0i)->values[d0Index]
                        + u1 * *GetBlock(i0i, j0i, k1i)->values[d0Index])
                        + ( t1 * (u0 * *GetBlock(i0i, j1i, k0i)->values[d0Index]
                        + u1 * *GetBlock(i0i, j1i, k1i)->values[d0Index])))
                        + s1 * ( t0 * (u0 * *GetBlock(i1i, j0i, k0i)->values[d0Index]
                        + u1 * *GetBlock(i1i, j0i, k1i)->values[d0Index])
                        + ( t1 * (u0 * *GetBlock(i1i, j1i, k0i)->values[d0Index]
                        + u1 * *GetBlock(i1i, j1i, k1i)->values[d0Index])));
            }
        }
    }
    SetBoundary(specialB, dIndex);
}


