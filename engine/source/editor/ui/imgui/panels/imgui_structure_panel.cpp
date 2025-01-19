//
// Created by Akira Mujawar on 13/07/24.
//

#include "imgui_structure_panel.hpp"

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

using MeowEngine::graphics::ui::ImGuiStructurePanel;

ImGuiStructurePanel::ImGuiStructurePanel()
: DefaultSelectableFlags(ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth)
, DefaultSelectableNoListFlags(DefaultSelectableFlags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen)
//, SelectionMask(1 << 2)
, IsActive(true)
, WindowFlags(ImGuiWindowFlags_NoCollapse) {}

MeowEngine::graphics::ui::ImGuiStructurePanel::~ImGuiStructurePanel() {

}

void ImGuiStructurePanel::Draw(entt::registry& registry) {
//    auto registers = scene.GetEntities();

    ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);

    ImGui::Begin("Structure", &IsActive); {
        auto view = registry.view<MeowEngine::entity::LifeObjectComponent>();
        for(auto entity: view)
        {
            auto& lifeObject =  view.get<MeowEngine::entity::LifeObjectComponent>(entity);
            CreateSelectableItem(
                registry,
                lifeObject,
                entity
            );
        }

        ImGui::End();
    }

}

void ImGuiStructurePanel::CreateSelectableItem(entt::registry& registry, MeowEngine::entity::LifeObjectComponent& lifeObject, entt::entity item) {
//    auto registers = scene.GetEntities();

//    const int id = lifeObject.Id;
    const int id = static_cast<int>(item);

//    const bool isSelected = registry.valid(SelectedEntity) && registry.get<MeowEngine::entity::LifeObjectComponent>(SelectedEntity).Id == id;
    const bool isSelected = registry.valid(SelectedEntity) && static_cast<int>(SelectedEntity) == id;
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
        MeowEngine::Log("Object Selected: ", std::to_string(id));
    }
}

entt::entity ImGuiStructurePanel::GetSelectedItem() {
    return SelectedEntity;
}
