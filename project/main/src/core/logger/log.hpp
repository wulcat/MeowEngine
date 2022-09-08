//
// Created by Akira Lynn on 06/07/22.
//

#ifndef VULKAN_ENGINE_LOG_HPP
#define VULKAN_ENGINE_LOG_HPP

#pragma once

#include <string>

namespace physicat {
    void Log(const std::string& tag, const std::string& message);
    void Log(const std::string& tag, const std::string& message, const std::exception& error);
} // namespace physicat


#endif //VULKAN_ENGINE_LOG_HPP
