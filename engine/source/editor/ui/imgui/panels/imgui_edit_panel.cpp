//
// Created by Akira Mujawar on 13/07/24.
//

#include <transform3d_component.hpp>
#include "imgui_edit_panel.hpp"
#include "imgui_wrapper.hpp"
#include "life_object_component.hpp"
#include "log.hpp"

physicat::graphics::ui::ImGuiEditPanel::ImGuiEditPanel() {

}

physicat::graphics::ui::ImGuiEditPanel::~ImGuiEditPanel() {

}

void physicat::graphics::ui::ImGuiEditPanel::Draw(physicat::Scene &scene, entt::entity lifeObject) {
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("Edit", &CanDrawPanel);
    {
        auto registry = scene.GetEntities();

        // NOTE: There's a issue when a edit is made to edit panel and a new item is selected without loosing focus from edit panel
        if(registry->valid(lifeObject))
        {
            auto& transform = registry->get<physicat::core::component::Transform3DComponent>(lifeObject);
            ImGui::InputFloat3("Position", &transform.Position[0]);
        }
        else
        {
            physicat::Log("Selected Entity: ", "Entity not valid");
        }

        ImGui::End();
    }
}
