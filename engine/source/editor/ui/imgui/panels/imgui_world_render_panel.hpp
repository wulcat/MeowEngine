//
// Created by Akira Mujawar on 14/07/24.
//

#ifndef MEOWENGINE_IMGUI_WORLD_RENDER_PANEL_HPP
#define MEOWENGINE_IMGUI_WORLD_RENDER_PANEL_HPP

#include "imgui_wrapper.hpp"
#include "window_size.hpp"

namespace MeowEngine::editor {

    struct ImGuiWorldRenderPanel {
        ImGuiWorldRenderPanel();
        ~ImGuiWorldRenderPanel();

        void Draw(void* frameBufferId, const float& inFps);

    private:
        bool IsActive;
        bool IsFocused; // soon come up with good naming conventions
        ImGuiWindowFlags WindowFlags;
//        int LastFPS;

        WindowSize SceneViewportSize;
    };
}


#endif //MEOWENGINE_IMGUI_WORLD_RENDER_PANEL_HPP
