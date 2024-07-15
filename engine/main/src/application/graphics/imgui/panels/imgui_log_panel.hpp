//
// Created by Akira Mujawar on 14/07/24.
//

#ifndef PHYSICAT_IMGUI_LOG_PANEL_HPP
#define PHYSICAT_IMGUI_LOG_PANEL_HPP

#include "imgui_wrapper.hpp"

namespace physicat::editor {
    struct ImGuiLogPanel {
        ImGuiLogPanel();
        ~ImGuiLogPanel();

        void Draw();

    private:
        bool IsActive;
        ImGuiWindowFlags WindowFlags;
    };
}


#endif //PHYSICAT_IMGUI_LOG_PANEL_HPP
