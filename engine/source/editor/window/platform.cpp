//
// Created by Akira Lynn on 05/07/22.
//

#include "platform.hpp"

MeowEngine::Platform MeowEngine::GetCurrentPlatform() {
    #if defined(__EMSCRIPTEN__)
        return MeowEngine::Platform::emscripten;
    #elif __APPLE__
        #include "TargetConditionals.h"
        #if TARGET_OS_IPHONE
            return MeowEngine::Platform::ios;
        #else
            return MeowEngine::Platform::mac;
        #endif
    #elif __ANDROID__
        return MeowEngine::Platform::android;
    #elif WIN32
        return MeowEngine::Platform::windows;
    #endif
}