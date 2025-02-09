//
// Created by Akira Mujawar on 06/07/22.
//

#ifndef VULKAN_ENGINE_LOG_HPP
#define VULKAN_ENGINE_LOG_HPP

#pragma once

#include <string>
#include "tracy_wrapper.hpp"

namespace MeowEngine {
    // Things we need
    // Basic logger while building
    // Logger to track for performance (merging with tracy profiler)
    // In-game logger to debug logs specific to game code

    void Log(const std::string& tag, const std::string& message);
    void Log(const std::string& tag, const int& message);
    void Log(const std::string& tag, const float& message);
    void Log(const std::string& tag, const bool& message);

    void Log(const std::string& tag, const std::string& message, const std::exception& error);
} // namespace MeowEngine


#endif //VULKAN_ENGINE_LOG_HPP
