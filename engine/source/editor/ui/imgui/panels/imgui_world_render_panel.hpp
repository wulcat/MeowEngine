//
// Created by Akira Mujawar on 14/07/24.
//

#ifndef PHYSICAT_IMGUI_WORLD_RENDER_PANEL_HPP
#define PHYSICAT_IMGUI_WORLD_RENDER_PANEL_HPP

#include "imgui_wrapper.hpp"
#include "window_size.hpp"

namespace physicat::editor {

    struct ImGuiWorldRenderPanel {
        ImGuiWorldRenderPanel();
        ~ImGuiWorldRenderPanel();

        void Draw(void* frameBufferId);

    private:
        bool IsActive;
        bool IsFocused; // soon come up with good naming conventions
        ImGuiWindowFlags WindowFlags;

        WindowSize SceneViewportSize;
    };
}


#endif //PHYSICAT_IMGUI_WORLD_RENDER_PANEL_HPP
