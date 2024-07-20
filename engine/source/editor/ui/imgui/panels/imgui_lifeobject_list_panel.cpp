//
// Created by Akira Mujawar on 13/07/24.
//

#include "imgui_lifeobject_list_panel.hpp"

//#include <SDL_video.h>
//#include <SDL_events.h>
//
//#include <sys/wait.h> // For waitpid()
//#include <unistd.h> // For fork(), exec()
//#include <csignal> // For signal handling
//
#include <log.hpp>

//#include "imgui_renderer.hpp"
//#include "bridge_wrapper.hpp"

using physicat::graphics::ui::ImGuiStructurePanel;

ImGuiStructurePanel::ImGuiStructurePanel()
: DefaultSelectableFlags(ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth)
, DefaultSelectableNoListFlags(DefaultSelectableFlags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen)
//, SelectionMask(1 << 2)
, IsActive(true)
, WindowFlags(ImGuiWindowFlags_NoCollapse)
, SelectedItem(nullptr) {}

physicat::graphics::ui::ImGuiStructurePanel::~ImGuiStructurePanel() {

}

void ImGuiStructurePanel::Draw(physicat::Scene &scene) {
    auto lifeObjects = scene.GetLifeObjects();

    ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);

//    int selectionMask = DefaultSelectionMask;

//    SelectionMask = (1 << 2);
    ImGui::Begin("Scene Objects", &IsActive); {
//        core::LifeObject* itemClicked = nullptr;

        for(int i = 0 ; i < lifeObjects.size(); i++) {
            CreateSelectableItem(lifeObjects[i]);
        }
//        int node_clicked = -1;
//        static int selection_mask = (1 << 2);
//        static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
//        for (int i = 0; i < 6; i++) {
//            ImGuiTreeNodeFlags node_flags = base_flags;
//            bool is_selected = (selection_mask & (1 << i)) != 0;
//            if (is_selected) {
////                    physicat::Log("Is i Selected: ", std::to_string(i));
//                node_flags |= ImGuiTreeNodeFlags_Selected;
//            }
//
//            bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Node %d", i);
//            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
//                node_clicked = i;
//
//
//            if (node_open)
//            {
//                ImGuiTreeNodeFlags node_flags1 = base_flags;
//                int uid = i + 7;
//                is_selected = (selection_mask & (1 << uid)) != 0;
//                if (is_selected) {
////                        physicat::Log("Is UID Selected: ", std::to_string(uid));
//                    node_flags1 |= ImGuiTreeNodeFlags_Selected;
//                }
//
//                node_flags1 |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
//                ImGui::TreeNodeEx((void*)(intptr_t)uid, node_flags1, "Selectable Leaf %d", uid);
//                if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
//                    node_clicked = uid;
//                ImGui::TreePop();
//            }
//        }

//        if (itemClicked)
//        {
//            physicat::Log("Object Selected: ", std::to_string(SelectedItem->Id));
//            // Update selection state
//            // (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
////            if (ImGui::GetIO().KeyCtrl)
////                SelectionMask ^= (1 << node_clicked);          // CTRL+click to toggle
////            else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, may want to preserve selection when clicking on item that is part of the selection
//                SelectionMask = (1 <<  itemClicked->Id);           // Click to single-select
//
//
//        }

        ImGui::End();
    }
}

void ImGuiStructurePanel::CreateSelectableItem(core::LifeObject* item) {
    const int id = item->Id;
    const bool isSelected = SelectedItem != nullptr && SelectedItem->Id ==id;
    ImGuiTreeNodeFlags flags = item->GetChildCount() == 0 ? DefaultSelectableNoListFlags : DefaultSelectableFlags;

    // If the item is selected we add selected flag
    if (isSelected) {
        flags |= ImGuiTreeNodeFlags_Selected;
    }

    // Draw the item
    bool isItemOpen = ImGui::TreeNodeEx((void*)(intptr_t)id, flags, item->Name.c_str());

    // If item gets clicked with cache item
    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
        SelectedItem = item ;
        physicat::Log("Object Selected: ", std::to_string(SelectedItem->Id));
    }

//    // For each child draw more child selectables
//    if(item.GetChildCount() > 0) {
//        CreateSelectableItem()
//    }
}

physicat::core::LifeObject*  ImGuiStructurePanel::GetSelectedItem() {
    return SelectedItem;
}
