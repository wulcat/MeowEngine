//
// Created by Akira Mujawar on 13/07/24.
//

#ifndef PHYSICAT_IMGUI_EDIT_PANEL_HPP
#define PHYSICAT_IMGUI_EDIT_PANEL_HPP

#include "life_object.hpp"
#include "math_wrapper.hpp"

namespace physicat::graphics::ui {
    struct ImGuiEditPanel {
        ImGuiEditPanel();
        ~ImGuiEditPanel();

        void Draw(core::LifeObject* lifeObject);

    private:
        bool CanDrawPanel;
    };
}


#endif //PHYSICAT_IMGUI_EDIT_PANEL_HPP
