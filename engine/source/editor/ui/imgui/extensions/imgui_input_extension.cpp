//
// Created by Akira Mujawar on 10/01/25.
//

#include "imgui_input_extension.hpp"
#include "imgui_wrapper.hpp"
#include "entt_reflection_wrapper.hpp"

#include "pstring.hpp"
#include "vector3.hpp"

void physicat::ImGuiInputExtension::ShowProperty(const std::string& inClassName, void* inObject) {
    std::vector<physicat::ReflectionProperty> properties = ReflectionTest.GetProperties(inClassName);

    // Display Component Properties
    for (const auto &property: properties) {
        switch (property.Type) {
            case physicat::NOT_DEFINED:
                break;
            case physicat::PRIMITIVE: {
                ImGui::Indent();
                ImGuiInputExtension::ShowPrimitive(property, inObject);
                ImGui::Unindent();
                break;
            }
            case physicat::ARRAY:
                break;
            case physicat::POINTER:
                break;
            case physicat::ENUM:
                break;
            case physicat::CLASS_OR_STRUCT: {
                ImGui::Indent();
                ImGuiInputExtension::ShowClassOrStruct(property, inObject);
                ImGui::Unindent();
                break;
            }
        }
    }
}

void physicat::ImGuiInputExtension::ShowPrimitive(const physicat::ReflectionProperty& inProperty, void* inObject) {
    if(inProperty.TypeId == typeid(int)) {
        void* value = inProperty.Get(inObject);

        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", inProperty.Name.c_str());
        ImGui::SameLine();
        ImGui::SetCursorPosX(200);
        ImGui::InputScalar("##hidden_label", ImGuiDataType_U32, value, nullptr, nullptr);
    }
    else if(inProperty.TypeId == typeid(float)) {
        void* value = inProperty.Get(inObject);

        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", inProperty.Name.c_str());
        ImGui::SameLine();
        ImGui::SetCursorPosX(200);
        ImGui::InputScalar("##hidden_label", ImGuiDataType_Float, value, nullptr, nullptr);
    }
}

void physicat::ImGuiInputExtension::ShowClassOrStruct(const physicat::ReflectionProperty& inProperty, void* inObject) {
    if(inProperty.TypeId == typeid(physicat::PString)) {
        physicat::PString& value = *static_cast<physicat::PString*>(inProperty.Get(inObject));

        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", inProperty.Name.c_str());
        ImGui::SameLine();
        ImGui::SetCursorPosX(200);
        ImGui::InputText("##hidden_label", value.data(), sizeof(value));
    }
    else if(inProperty.TypeId == typeid(physicat::math::Vector3)) {
        physicat::math::Vector3& value = *static_cast<physicat::math::Vector3*>(inProperty.Get(inObject));

        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", inProperty.Name.c_str());
        ImGui::SameLine();
        ImGui::SetCursorPosX(200);
        ImGui::InputFloat3("##hidden_label", &value[0]);
    }
    else {
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if(ImGui::TreeNode(inProperty.Name.c_str())) {
            ShowProperty(inProperty.TypeName, inProperty.Get(inObject));
            ImGui::TreePop();
        }
    }
}

void physicat::ImGuiInputExtension::ShowTabExample() {
    if (ImGui::BeginTable("LayoutTable", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_NoPadInnerX)) {
        ImGui::TableSetupColumn("LabelColumn", ImGuiTableColumnFlags_WidthFixed,
                                100.0f); // Label column width
        ImGui::TableSetupColumn("InputColumn");       // Input column stretches

        ImGui::TableNextColumn();
        ImGui::Text("Label:");
        ImGui::TableNextColumn();
        char buffer[128] = "Default Text";
        ImGui::InputText("##input", buffer, sizeof(buffer));
//                    ImGui::InputFloat3("##hidden_label", &value[0]);

        ImGui::EndTable();
    }
}

void physicat::ImGuiInputExtension::ShowPushItemWidthExample() {
    if (ImGui::Begin("PushItemWidth Example")) {
        char buffer1[128] = "Default Text";
        char buffer2[128] = "Default Text";
        // Set width for the next input field
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.5f); // 50% of available width
        ImGui::InputText("Input 1", buffer1, sizeof(buffer1));
        ImGui::PopItemWidth();

        ImGui::SameLine();

        // Set width for another input field
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.3f); // 30% of remaining width
        ImGui::InputText("Input 2", buffer2, sizeof(buffer2));
        ImGui::PopItemWidth();
    }
    ImGui::End();
}

void physicat::ImGuiInputExtension::TextInputWithLimitedText(const char* label, float textPercentage, char* inputBuffer, size_t bufferSize) {
    float totalWidth = ImGui::GetContentRegionAvail().x; // Available panel width
    float textWidth = totalWidth * textPercentage;      // Width for the text
    float inputWidth = totalWidth - textWidth;          // Remaining width for the input

    physicat::Log("Test", totalWidth);
    // Render the text constrained to the specified width
    ImGui::PushItemWidth(100);
    ImGui::TextUnformatted(label);
    ImGui::PopItemWidth();

    // Place the input field on the same line, occupying the remaining space
    ImGui::SameLine();
    ImGui::PushItemWidth(inputWidth);
    ImGui::InputText("##input", inputBuffer, bufferSize); // Ensure a unique ID (##input)
    ImGui::PopItemWidth();
}

void physicat::ImGuiInputExtension::TextInputWithLimitedTextWrap(const char* label, float textPercentage, char* inputBuffer, size_t bufferSize) {
    float totalWidth = ImGui::GetContentRegionAvail().x; // Available width in the current layout
    float textWidth = totalWidth * textPercentage;      // Calculate text width
    float inputWidth = totalWidth - textWidth;          // Calculate input field width

    // Render the text within the calculated width
    ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + textWidth); // Wrap text to fit width
    ImGui::TextUnformatted(label);
    ImGui::PopTextWrapPos();

    // Move to the same line and adjust for input field position
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + textWidth); // Ensure proper alignment
    ImGui::PushItemWidth(inputWidth); // Set the input field width
    ImGui::InputText("##input", inputBuffer, bufferSize);
    ImGui::PopItemWidth();
}

void physicat::ImGuiInputExtension::SeparatorTest() {
//            ImGui::SeparatorText("");
//            ImGui::AlignTextToFramePadding();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Separator();
//            ImGui::PushStyleVar(ImGuiStyleVar_F, ImGui::GetIO().Fonts->Fonts[1]);  // Use a bold font
//            ImGui::SeparatorText(className.c_str());
    ImGui::Text("--%s", "className.c_str()");
//            ImGui::PopStyleVar();  // Restore default font style
//            ImGui::AlignTextToFramePadding();
    ImGui::Separator();
    ImGui::Spacing();
}