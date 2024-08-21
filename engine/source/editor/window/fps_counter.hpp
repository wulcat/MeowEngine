//
// Created by Akira Mujawar on 18/08/24.
//

#ifndef PHYSICAT_FPS_COUNTER_HPP
#define PHYSICAT_FPS_COUNTER_HPP

#include "vector";
#include "sdl_wrapper.hpp"

class FpsCounter {
public:
    FpsCounter(int sampleSize = 100)
            : sampleSize(sampleSize), frameTimes(sampleSize), currentIndex(0), accumulatedTime(0.0) {}

    void frameStart() {
        frameStartTime = SDL_GetPerformanceCounter();
    }

    void frameEnd() {
        Uint64 frameEndTime = SDL_GetPerformanceCounter();
        double deltaTime = (double)(frameEndTime - frameStartTime) / SDL_GetPerformanceFrequency();

        accumulatedTime += deltaTime - frameTimes[currentIndex];
        frameTimes[currentIndex] = deltaTime;
        currentIndex = (currentIndex + 1) % sampleSize;
    }

    double getSmoothFPS() const {
        double averageFrameTime = accumulatedTime / sampleSize;
        return averageFrameTime > 0.0 ? 1.0 / averageFrameTime : 0.0;
    }

private:
    int sampleSize;
    std::vector<double> frameTimes;
    int currentIndex;
    double accumulatedTime;
    Uint64 frameStartTime;
};


#endif //PHYSICAT_FPS_COUNTER_HPP
