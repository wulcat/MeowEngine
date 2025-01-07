//
// Created by Akira Mujawar on 13/07/24.
//

#ifndef PHYSICAT_IMGUI_STRUCTURE_PANEL_HPP
#define PHYSICAT_IMGUI_STRUCTURE_PANEL_HPP

//#include <scene.hpp>
//#include "opengl_framebuffer.hpp"
//#include "window_size.hpp"
#include "imgui_wrapper.hpp"
#include "life_object_component.hpp"
#include "entt_wrapper.hpp"

namespace physicat::graphics::ui {
    struct ImGuiStructurePanel {
        ImGuiStructurePanel();
        ~ImGuiStructurePanel();

        void Draw(entt::registry& registry);

        void CreateSelectableItem(entt::registry& registry, physicat::entity::LifeObjectComponent& lifeObject, entt::entity item);

        // Returns true if item is selected
        entt::entity GetSelectedItem();

    private:
        const ImGuiTreeNodeFlags DefaultSelectableFlags;
        const ImGuiTreeNodeFlags DefaultSelectableNoListFlags;
        int SelectionMask; // TODO: try to understand the logic

        bool IsActive;
        ImGuiWindowFlags WindowFlags;
//        std::weak_ptr<core::LifeObject> SelectedItem;
//        core::LifeObject* SelectedItem;
        entt::entity SelectedEntity;
    };
}


#endif //PHYSICAT_IMGUI_STRUCTURE_PANEL_HPP
