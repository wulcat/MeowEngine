//
// Created by Akira Mujawar on 14/07/24.
//

#include "imgui_world_render_panel.hpp"

#include "log.hpp"
#include "window_size.hpp"
#include <SDL_events.h>

using physicat::editor::ImGuiWorldRenderPanel;

ImGuiWorldRenderPanel::ImGuiWorldRenderPanel()
    : WindowFlags(ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_UnsavedDocument)
    , IsActive(false)
    , IsFocused(true) {
    PT_PROFILE_ALLOC("ImGuiWorldRenderPanel", sizeof(ImGuiWorldRenderPanel));
}

ImGuiWorldRenderPanel::~ImGuiWorldRenderPanel() {
    PT_PROFILE_FREE("ImGuiWorldRenderPanel");
}

void ImGuiWorldRenderPanel::Draw(void* frameBufferId) {

    ImGui::Begin("Scene", &IsActive,WindowFlags); {
        const bool isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);
        if(isFocused != IsFocused) {
            IsFocused = isFocused;

            SDL_Event event;
            SDL_zero(event);
            event.type = SDL_USEREVENT;
            event.user.code = 3;
            event.user.data1 = &IsFocused;

            SDL_PushEvent(&event);
        }

        ImGui::BeginChild("GameRender");

        const ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        if((uint32_t)viewportSize.x != SceneViewportSize.Width || (uint32_t)viewportSize.y != SceneViewportSize.Height) {
            SceneViewportSize.Width = (uint32_t)viewportSize.x;
            SceneViewportSize.Height = (uint32_t)viewportSize.y;

            SDL_Event event;
            SDL_zero(event);
            event.type = SDL_USEREVENT;
            event.user.code = 2;
            event.user.data1 = &SceneViewportSize;

            SDL_PushEvent(&event);
        }

        ImGui::Image(
            (ImTextureID)frameBufferId,
            ImGui::GetContentRegionAvail(),
            ImVec2(0, 1),
            ImVec2(1, 0)
        );
    }

    ImGui::EndChild();
    ImGui::End();
}
