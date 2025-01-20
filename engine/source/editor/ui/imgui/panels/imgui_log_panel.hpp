//
// Created by Akira Mujawar on 14/07/24.
//

#ifndef MEOWENGINE_IMGUI_LOG_PANEL_HPP
#define MEOWENGINE_IMGUI_LOG_PANEL_HPP

#include "imgui_wrapper.hpp"

namespace MeowEngine::editor {
    struct ImGuiLogPanel {
        ImGuiLogPanel();
        ~ImGuiLogPanel();

        void Draw();

    private:
        bool IsActive;
        ImGuiWindowFlags WindowFlags;
    };
}


#endif //MEOWENGINE_IMGUI_LOG_PANEL_HPP
