//
// Created by Akira Mujawar on 13/07/24.
//

#ifndef MEOWENGINE_IMGUI_EDIT_PANEL_HPP
#define MEOWENGINE_IMGUI_EDIT_PANEL_HPP

#include "math_wrapper.hpp"
//#include "scene.hpp"
#include "entt_wrapper.hpp"
#include "reflection_property_change.hpp"
#include "queue"

namespace MeowEngine::graphics::ui {
    struct ImGuiEditPanel {
        ImGuiEditPanel();
        ~ImGuiEditPanel();

        void Draw(entt::registry& registry, std::queue<std::shared_ptr<MeowEngine::ReflectionPropertyChange>>& inUIInputQueue, entt::entity lifeObject);

    private:
        bool CanDrawPanel;
    };
}


#endif //MEOWENGINE_IMGUI_EDIT_PANEL_HPP
