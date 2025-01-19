//
// Created by Akira Lynn on 05/07/22.
//



#pragma once

namespace MeowEngine
{
    enum class Platform
    {
        ios,
        android,
        windows,
        mac,
        emscripten
    };

    Platform GetCurrentPlatform();
} // namespace MeowEngine

