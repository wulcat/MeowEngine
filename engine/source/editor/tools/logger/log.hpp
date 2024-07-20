//
// Created by Akira Lynn on 06/07/22.
//

#ifndef VULKAN_ENGINE_LOG_HPP
#define VULKAN_ENGINE_LOG_HPP

#pragma once

#include <string>
#include "tracy_wrapper.hpp"

namespace physicat {
    // Things we need
    // Basic logger while building
    // Logger to track for performance (merging with tracy profiler)
    // In-game logger to debug logs specific to game code

    void Log(const std::string& tag, const std::string& message);
    void Log(const std::string& tag, const std::string& message, const std::exception& error);
} // namespace physicat


#endif //VULKAN_ENGINE_LOG_HPP
