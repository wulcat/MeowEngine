//
// Created by Akira Mujawar on 10/01/25.
//

#include "imgui_input_extension.hpp"
#include "imgui_wrapper.hpp"
#include "entt_reflection_wrapper.hpp"

#include "pstring.hpp"
#include "vector3.hpp"

MeowEngine::ReflectionPropertyChange* MeowEngine::ImGuiInputExtension::ShowProperty(const std::string& inClassName, void* inObject) {
    std::vector<MeowEngine::ReflectionProperty> properties = MeowEngine::Reflection.GetProperties(inClassName);
    MeowEngine::ReflectionPropertyChange* change = nullptr;

    // Display Component Properties
    for (const auto &property: properties) {
        switch (property.Type) {
            case MeowEngine::NOT_DEFINED:
                break;
            case MeowEngine::PRIMITIVE: {
                ImGui::Indent();
                MeowEngine::ReflectionPropertyChange::Assign(change, ImGuiInputExtension::ShowPrimitive(property, inObject));
                ImGui::Unindent();
                break;
            }
            case MeowEngine::ARRAY:
                break;
            case MeowEngine::POINTER:
                break;
            case MeowEngine::ENUM:
                break;
            case MeowEngine::CLASS_OR_STRUCT: {
                ImGui::Indent();
                MeowEngine::ReflectionPropertyChange::Assign(change, ImGuiInputExtension::ShowClassOrStruct(property, inObject));
                ImGui::Unindent();
                break;
            }
        }
    }

    return change;
}

MeowEngine::ReflectionPropertyChange* MeowEngine::ImGuiInputExtension::ShowPrimitive(const MeowEngine::ReflectionProperty& inProperty, void* inObject) {
    MeowEngine::ReflectionPropertyChange* change = nullptr;

    if(inProperty.TypeId == typeid(int)) {
        void* value = inProperty.Get(inObject);
        int changeHolder = *static_cast<int*>(value);
        auto uniqueId = reinterpret_cast<uintptr_t>(value);

        std::string labelName = MeowEngine::PString::Format("##%s", inProperty.Name.c_str(), std::to_string(uniqueId).c_str());

        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", inProperty.Name.c_str());
        ImGui::SameLine();
        ImGui::SetCursorPosX(200);

        if(ImGui::InputScalar(labelName.c_str(), ImGuiDataType_U32, &changeHolder, nullptr, nullptr, nullptr, ImGuiInputTextFlags_EnterReturnsTrue)) {
            change = new MeowEngine::ReflectionPropertyChange(inProperty.Name, new int(changeHolder), [](void* inPointer){ delete static_cast<int*>(inPointer); });
        }
    }
    else if(inProperty.TypeId == typeid(float)) {
        void* value = inProperty.Get(inObject);
        float changeHolder = *static_cast<float*>(value);
        auto uniqueId = reinterpret_cast<uintptr_t>(value);

        std::string labelName = MeowEngine::PString::Format("##%s", inProperty.Name.c_str(), std::to_string(uniqueId).c_str());

        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", inProperty.Name.c_str());
        ImGui::SameLine();
        ImGui::SetCursorPosX(200);

        if(ImGui::InputScalar(labelName.c_str(), ImGuiDataType_Float, &changeHolder, nullptr, nullptr, nullptr, ImGuiInputTextFlags_EnterReturnsTrue)) {
            change = new MeowEngine::ReflectionPropertyChange(inProperty.Name, new float(changeHolder), [](void* inPointer){ delete static_cast<float*>(inPointer); });
        }
    }

    return change;
}

MeowEngine::ReflectionPropertyChange* MeowEngine::ImGuiInputExtension::ShowClassOrStruct(const MeowEngine::ReflectionProperty& inProperty, void* inObject) {
    MeowEngine::ReflectionPropertyChange* change = nullptr;

    if(inProperty.TypeId == typeid(MeowEngine::PString)) {
        void* value = inProperty.Get(inObject);
        MeowEngine::PString changeHolder = *static_cast<MeowEngine::PString*>(value);
        auto uniqueId = reinterpret_cast<uintptr_t>(value);

        std::string labelName = MeowEngine::PString::Format("##%s", inProperty.Name.c_str(), std::to_string(uniqueId).c_str());

        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", inProperty.Name.c_str());
        ImGui::SameLine();
        ImGui::SetCursorPosX(200);

        if(ImGui::InputText(labelName.c_str(), changeHolder.data(), 32, ImGuiInputTextFlags_EnterReturnsTrue)) {
            change = new MeowEngine::ReflectionPropertyChange(inProperty.Name, new MeowEngine::PString(changeHolder), [](void* inPointer){ delete static_cast<MeowEngine::PString*>(inPointer); });
        }
    }
    else if(inProperty.TypeId == typeid(MeowEngine::math::Vector3)) {
        void* value = inProperty.Get(inObject);
        MeowEngine::math::Vector3 changeHolder = *static_cast<MeowEngine::math::Vector3*>(value);
        auto uniqueId = reinterpret_cast<uintptr_t>(value);

        std::string labelName = MeowEngine::PString::Format("##%s", inProperty.Name.c_str(), std::to_string(uniqueId).c_str());

        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", inProperty.Name.c_str());
        ImGui::SameLine();
        ImGui::SetCursorPosX(200);

        if(ImGui::InputFloat3(labelName.c_str(), &changeHolder[0], nullptr, ImGuiInputTextFlags_EnterReturnsTrue)) {
            change = new MeowEngine::ReflectionPropertyChange(inProperty.Name, new MeowEngine::math::Vector3(changeHolder), [](void* inPointer){ delete static_cast<MeowEngine::math::Vector3*>(inPointer); });
        }
    }
    else {
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);

        if(ImGui::TreeNode(inProperty.Name.c_str())) {
            MeowEngine::ReflectionPropertyChange::Assign(change, ShowProperty(inProperty.TypeName, inProperty.Get(inObject)));

            if(change != nullptr) {
                change->ClassProperties.push_back(inProperty);
            }

            ImGui::TreePop();
        }
    }

    return change;
}

void MeowEngine::ImGuiInputExtension::ShowTabExample() {
    if (ImGui::BeginTable("LayoutTable", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_NoPadInnerX)) {
        ImGui::TableSetupColumn("LabelColumn", ImGuiTableColumnFlags_WidthFixed,
                                100.0f); // Label column width
        ImGui::TableSetupColumn("InputColumn");       // Input column stretches

        ImGui::TableNextColumn();
        ImGui::Text("Label:");
        ImGui::TableNextColumn();
        char buffer[128] = "Default Text";
        ImGui::InputText("##input", buffer, sizeof(buffer));
//                    ImGui::InputFloat3("##hidden_labedl", &value[0]);

        ImGui::EndTable();
    }
}

void MeowEngine::ImGuiInputExtension::ShowPushItemWidthExample() {
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

void MeowEngine::ImGuiInputExtension::TextInputWithLimitedText(const char* label, float textPercentage, char* inputBuffer, size_t bufferSize) {
    float totalWidth = ImGui::GetContentRegionAvail().x; // Available panel width
    float textWidth = totalWidth * textPercentage;      // Width for the text
    float inputWidth = totalWidth - textWidth;          // Remaining width for the input

    MeowEngine::Log("Test", totalWidth);
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

void MeowEngine::ImGuiInputExtension::TextInputWithLimitedTextWrap(const char* label, float textPercentage, char* inputBuffer, size_t bufferSize) {
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

void MeowEngine::ImGuiInputExtension::SeparatorTest() {
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