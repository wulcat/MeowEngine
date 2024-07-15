//
// Created by Akira Mujawar on 14/07/24.
//

#include "imgui_log_panel.hpp"
#include "log.hpp"

using physicat::editor::ImGuiLogPanel;

ImGuiLogPanel::ImGuiLogPanel()
    : WindowFlags(ImGuiWindowFlags_NoCollapse)
    , IsActive(false) {
    PT_PROFILE_ALLOC("ImGuiLogPanel", sizeof(ImGuiLogPanel))
}

ImGuiLogPanel::~ImGuiLogPanel() {
    PT_PROFILE_FREE("ImGuiLogPanel")
}

void ImGuiLogPanel::Draw() {
    ImGui::Begin("Logs", &IsActive);
    {
        ImGui::Text("Demo Log 1");
        ImGui::Text("Demo Log 2");

        ImGui::End();
    }
}
