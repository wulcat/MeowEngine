//
// Created by Akira Mujawar on 18/08/24.
//

#ifndef MEOWENGINE_FPS_COUNTER_HPP
#define MEOWENGINE_FPS_COUNTER_HPP

#include "vector"
#include "sdl_wrapper.hpp"

namespace MeowEngine {
// NOTE: Revisit on this
    class FrameRateCounter {
    public:
        FrameRateCounter(float inFrameRate, int sampleSize = 100)
                : frameStartTime(SDL_GetPerformanceCounter()), targetFrameTime(1 / inFrameRate), sampleSize(sampleSize),
                  frameTimes(sampleSize),
                  currentIndex(0),
                  accumulatedTime(0.0),
                  DeltaTime(0.0) {}

        void Calculate() {
            // calculate smooth frame rate
            Uint64 frameEndTime = SDL_GetPerformanceCounter();
            double deltaTime = (double) (frameEndTime - frameStartTime) / SDL_GetPerformanceFrequency();

            accumulatedTime += deltaTime - frameTimes[currentIndex];
            frameTimes[currentIndex] = deltaTime;
            currentIndex = (currentIndex + 1) % sampleSize;

            // calculate delta time
//        float elapsedTime = (frameEndTime - frameStartTime) * 1000.0f;
//        float deltaTime = (elapsedTime / FramePerSecond) * 0.001f;
            DeltaTime = deltaTime;

            // cache new frame time
            frameStartTime = SDL_GetPerformanceCounter();
        }

        void LockFrameRate() {
            Uint64 frameEndTime = SDL_GetPerformanceCounter();
            double frameDuration = (double) (frameEndTime - frameStartTime) / frequency;

            while (frameDuration < targetFrameTime) {
                frameEndTime = SDL_GetPerformanceCounter();
                frameDuration = (double) (frameEndTime - frameStartTime) / frequency;
            }

            previousTime = frameEndTime;
        }

        double GetFrameRate() const {
            double averageFrameTime = accumulatedTime / sampleSize;
            return averageFrameTime > 0.0 ? 1.0 / averageFrameTime : 0.0;
        }

        float DeltaTime;
    private:
        // for getting smooth frame rate
        int sampleSize;
        std::vector<double> frameTimes;
        int currentIndex;
        double accumulatedTime;
        Uint64 frameStartTime;

        // for locking frame rate
        double targetFrameTime;  // Targeting 60 FPS
        Uint64 frequency = SDL_GetPerformanceFrequency();
        Uint64 previousTime = SDL_GetPerformanceCounter();

        // for delta time
//    const float FramePerSecond; FramePerSecond(static_cast<float>(SDL_GetPerformanceFrequency())

    };
}

#endif //MEOWENGINE_FPS_COUNTER_HPP
