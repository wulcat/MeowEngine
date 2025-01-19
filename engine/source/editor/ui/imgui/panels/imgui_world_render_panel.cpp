//
// Created by Akira Mujawar on 14/07/24.
//

#include "imgui_world_render_panel.hpp"

#include "log.hpp"
#include "window_size.hpp"
#include <SDL_events.h>

using MeowEngine::editor::ImGuiWorldRenderPanel;

ImGuiWorldRenderPanel::ImGuiWorldRenderPanel()
    : WindowFlags(ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_UnsavedDocument)
    , IsActive(false)
    , IsFocused(true) {
    PT_PROFILE_ALLOC("ImGuiWorldRenderPanel", sizeof(ImGuiWorldRenderPanel));
}

ImGuiWorldRenderPanel::~ImGuiWorldRenderPanel() {
    PT_PROFILE_FREE("ImGuiWorldRenderPanel");
}

void ImGuiWorldRenderPanel::Draw(void* frameBufferId, const float& inFps) {

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

        int fontSize = 2;
//        float smoothing = 0.99f; // larger=more smoothing
        //float smoothing = std::pow(0.9, (int)(1 / inTime) * 60 / 1000);
//        LastFPS = (LastFPS * smoothing) + ((int)(1 / inTime) * (1.0-smoothing));

        const char* fpsText = std::to_string((int)inFps).c_str();
        float textWidth = ImGui::CalcTextSize(fpsText).x * fontSize; // Get the text width
        ImVec2 textPos = ImVec2(SceneViewportSize.Width - textWidth - ImGui::GetStyle().WindowPadding.x, ImGui::GetStyle().WindowPadding.y);
        ImGui::SetCursorPos(textPos);
        ImGui::SetWindowFontScale(fontSize);
        ImGui::Text("%s", fpsText);
        ImGui::SetWindowFontScale(1.0f);
    }

    ImGui::EndChild();
    ImGui::End();
}
