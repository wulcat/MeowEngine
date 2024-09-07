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
            entity::Transform3DComponent& transform = registry->get<physicat::entity::Transform3DComponent>(lifeObject);
            entity::RigidbodyComponent* rigidbody = registry->try_get<physicat::entity::RigidbodyComponent>(lifeObject);

            // If there's a rigidbody to avoid continuous transform update we add "enter" after edit's. Else it will auto update on edit.
            if(rigidbody) {
                ImGui::AlignTextToFramePadding();
                ImGui::Text("Position");
                ImGui::SameLine();
                if (ImGui::InputFloat3("##hidden_label", &transform.Position[0], "%.3f", ImGuiInputTextFlags_EnterReturnsTrue)) {
                    rigidbody->OverrideTransform(transform);
                }
            }
            else {
                ImGui::AlignTextToFramePadding();
                ImGui::Text("Position");
                ImGui::SameLine();
                ImGui::InputFloat3("##hidden_label", &transform.Position[0]);
            }
        }
        else
        {
            physicat::Log("Selected Entity: ", "Entity not valid");
        }

        ImGui::End();
    }
}
