//
// Created by Akira Mujawar on 13/07/24.
//

#ifndef PHYSICAT_IMGUI_LIFEOBJECT_LIST_PANEL_HPP
#define PHYSICAT_IMGUI_LIFEOBJECT_LIST_PANEL_HPP

#include <scene.hpp>
//#include "opengl_framebuffer.hpp"
//#include "window_size.hpp"
#include "imgui_wrapper.hpp"

namespace physicat::graphics::ui {
    struct ImGuiStructurePanel {
        ImGuiStructurePanel();
        ~ImGuiStructurePanel();

        void Draw(physicat::Scene &scene);

        void CreateSelectableItem(core::LifeObject* item);

        // Returns true if item is selected
        physicat::core::LifeObject* GetSelectedItem();

    private:
        const ImGuiTreeNodeFlags DefaultSelectableFlags;
        const ImGuiTreeNodeFlags DefaultSelectableNoListFlags;
        int SelectionMask; // TODO: try to understand the logic

        bool IsActive;
        ImGuiWindowFlags WindowFlags;
//        std::weak_ptr<core::LifeObject> SelectedItem;
        core::LifeObject* SelectedItem;
    };
}


#endif //PHYSICAT_IMGUI_LIFEOBJECT_LIST_PANEL_HPP
