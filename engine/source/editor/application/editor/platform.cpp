//
// Created by Akira Lynn on 05/07/22.
//

#include "platform.hpp"

physicat::Platform physicat::GetCurrentPlatform() {
    #if defined(__EMSCRIPTEN__)
        return physicat::Platform::emscripten;
    #elif __APPLE__
        #include "TargetConditionals.h"
        #if TARGET_OS_IPHONE
            return physicat::Platform::ios;
        #else
            return physicat::Platform::mac;
        #endif
    #elif __ANDROID__
        return physicat::Platform::android;
    #elif WIN32
        return physicat::Platform::windows;
    #endif
}