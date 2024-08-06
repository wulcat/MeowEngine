//
// Created by Akira Mujawar on 13/07/24.
//

#ifndef PHYSICAT_IMGUI_EDIT_PANEL_HPP
#define PHYSICAT_IMGUI_EDIT_PANEL_HPP

#include "math_wrapper.hpp"
#include "scene.hpp"

namespace physicat::graphics::ui {
    struct ImGuiEditPanel {
        ImGuiEditPanel();
        ~ImGuiEditPanel();

        void Draw(physicat::Scene &scene, entt::entity lifeObject);

    private:
        bool CanDrawPanel;
    };
}


#endif //PHYSICAT_IMGUI_EDIT_PANEL_HPP
