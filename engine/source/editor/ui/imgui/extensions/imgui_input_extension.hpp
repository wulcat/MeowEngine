//
// Created by Akira Mujawar on 10/01/25.
//

#ifndef PHYSICAT_IMGUI_INPUT_EXTENSION_HPP
#define PHYSICAT_IMGUI_INPUT_EXTENSION_HPP

#include "string"
#include "reflection_property.hpp"
#include "reflection_property_change.hpp"

using namespace std;

namespace MeowEngine {
    class ImGuiInputExtension {
    public:
        static MeowEngine::ReflectionPropertyChange* ShowProperty(const std::string& inClassName, void* inObject);
        static MeowEngine::ReflectionPropertyChange* ShowPrimitive(const MeowEngine::ReflectionProperty& inProperty, void* inObject);
        static MeowEngine::ReflectionPropertyChange* ShowClassOrStruct(const MeowEngine::ReflectionProperty& inProperty, void* inObject);

        static void ShowTabExample();
        static void ShowPushItemWidthExample();
        static void TextInputWithLimitedText(const char* label, float textPercentage, char* inputBuffer, size_t bufferSize);
        static void TextInputWithLimitedTextWrap(const char* label, float textPercentage, char* inputBuffer, size_t bufferSize);
        static void SeparatorTest();
    };
}

#endif //PHYSICAT_IMGUI_INPUT_EXTENSION_HPP
