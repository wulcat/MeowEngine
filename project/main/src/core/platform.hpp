//
// Created by Akira Lynn on 05/07/22.
//



#pragma once

namespace ast
{
    enum class Platform
    {
        mac,
        ios,
        android,
        emscripten
    };

    Platform getCurrentPlatform();
} // namespace ast

