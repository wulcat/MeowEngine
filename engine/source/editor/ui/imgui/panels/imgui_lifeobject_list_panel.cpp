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
, WindowFlags(ImGuiWindowFlags_NoCollapse) {}

physicat::graphics::ui::ImGuiStructurePanel::~ImGuiStructurePanel() {

}

void ImGuiStructurePanel::Draw(physicat::Scene &scene) {
    auto registers = scene.GetEntities();

    ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);

    ImGui::Begin("Scene Objects", &IsActive); {
        auto view = registers->view<physicat::entity::LifeObjectComponent>();
        for(auto entity: view)
        {
            auto lifeObject =  view.get<physicat::entity::LifeObjectComponent>(entity);
            CreateSelectableItem(
                scene,
                lifeObject,
                entity
            );
        }

        ImGui::End();
    }

}

void ImGuiStructurePanel::CreateSelectableItem(physicat::Scene &scene, physicat::entity::LifeObjectComponent& lifeObject, entt::entity item) {
    auto registers = scene.GetEntities();

    const int id = lifeObject.Id;
    const bool isSelected = registers->valid(SelectedEntity) && registers->get<physicat::entity::LifeObjectComponent>(SelectedEntity).Id == id;
    ImGuiTreeNodeFlags flags = lifeObject.GetChildCount() == 0 ? DefaultSelectableNoListFlags : DefaultSelectableFlags;

    // If the item is selected we add selected flag
    if (isSelected) {
        flags |= ImGuiTreeNodeFlags_Selected;
    }

    // Draw the item
    bool isItemOpen = ImGui::TreeNodeEx((void*)(intptr_t)id, flags, "%s", lifeObject.Name.c_str());

    // If item gets clicked with cache item
    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
        SelectedEntity = item ;
        physicat::Log("Object Selected: ", std::to_string(lifeObject.Id));
    }
}

entt::entity ImGuiStructurePanel::GetSelectedItem() {
    return SelectedEntity;
}
