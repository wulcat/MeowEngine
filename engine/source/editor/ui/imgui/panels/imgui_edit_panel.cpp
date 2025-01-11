//
// Created by Akira Mujawar on 13/07/24.
//

#include "imgui_edit_panel.hpp"
#include "imgui_wrapper.hpp"
#include "log.hpp"

#include "entt_reflection_wrapper.hpp"
#include "imgui_input_extension.hpp"

physicat::graphics::ui::ImGuiEditPanel::ImGuiEditPanel() {

}

physicat::graphics::ui::ImGuiEditPanel::~ImGuiEditPanel() {

}

void physicat::graphics::ui::ImGuiEditPanel::Draw(entt::registry& registry, entt::entity lifeObject) {
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("Edit", &CanDrawPanel);
    {
        // NOTE: There's a issue when a edit is made to edit panel and a new item is selected without loosing focus from edit panel
        if(registry.valid(lifeObject))
        {
            // for each component or
            for(pair<unsigned int, entt::basic_sparse_set<>&> component : registry.storage()){
                if(component.second.contains(lifeObject)) {
                    entt::id_type type = component.first;
                    const std::string componentName = physicat::Reflection.GetComponentName(type);
                    void* componentObject = component.second.value(lifeObject);

                    // Display Component Name
                    if(ImGui::CollapsingHeader(componentName.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                        if(
                            physicat::ReflectionPropertyChange* change = physicat::ImGuiInputExtension::ShowProperty(componentName, componentObject);
                            change != nullptr
                        ){
                            change->EntityId = static_cast<int>(lifeObject);
                            change->ComponentType = type;

//                            physicat::Log("Edit Panel", *static_cast<float*>(change->Data));
                            inUIInputQueue.push(*change);
                        }

                        ImGui::Spacing();
                    }
                }
            }

//            for(physicat::ReflectionProperty property : componentProperties) {
//                if(property.Name == propertyName) {
//                    // if last last propertyName
////                    property.Set(componentObject, propertyData);
////                      else property.Get() then move forward in loop
//                    break;
//                }
//            }

            // entt component id -> queue<property.name> -> value data pointer

//            entity::Transform3DComponent& transform = registry.get<physicat::entity::Transform3DComponent>(lifeObject);
//            entity::RigidbodyComponent* rigidbody = registry.try_get<physicat::entity::RigidbodyComponent>(lifeObject);

//            // If there's a rigidbody to avoid continuous transform update we add "enter" after edit's. Else it will auto update on edit.
//            if(rigidbody) {
//                ImGui::AlignTextToFramePadding();
//                ImGui::Text("Position");
//                ImGui::SameLine();
//                if (ImGui::InputFloat3("##hidden_label", &transform.Position[0], "%.3f", ImGuiInputTextFlags_EnterReturnsTrue)) {
//                    rigidbody->OverrideTransform(transform);
//                }
//            }
        }
        else
        {
            physicat::Log("Selected Entity: ", "Entity not valid");
        }

        ImGui::End();
    }
}
