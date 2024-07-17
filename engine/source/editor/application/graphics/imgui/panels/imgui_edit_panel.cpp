//
// Created by Akira Mujawar on 13/07/24.
//

#include <transform3d_component.hpp>
#include "imgui_edit_panel.hpp"
#include "imgui_wrapper.hpp"

physicat::graphics::ui::ImGuiEditPanel::ImGuiEditPanel() {

}

physicat::graphics::ui::ImGuiEditPanel::~ImGuiEditPanel() {

}

void physicat::graphics::ui::ImGuiEditPanel::Draw(physicat::core::LifeObject *lifeObject) {
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("Edit", &CanDrawPanel);
    {
        if(lifeObject) {
            ImGui::SeparatorText("Transform"); {
//            float value = 0;
//            ImGui::InputFloat("##value", &value, 1.0f);

                core::component::Transform3DComponent* transform = dynamic_cast<core::component::Transform3DComponent*>(lifeObject->TransformComponent);

                ImGui::InputFloat3("Position", &transform->Position[0]);
//                ImGui::InputFloat3("Scale", vec4a);
//                ImGui::InputFloat3("Rotation", vec4a);
            }
        }

        ImGui::End();
    }
}
