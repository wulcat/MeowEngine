//
// Created by Akira Mujawar on 08/07/24.
//

#ifndef PHYSICAT_IMGUI_RENDERER_HPP
#define PHYSICAT_IMGUI_RENDERER_HPP

#include <scene.hpp>
#include "opengl_framebuffer.hpp"
#include "window_size.hpp"

#include "imgui_structure_panel.hpp"
#include "imgui_edit_panel.hpp"
#include "imgui_world_render_panel.hpp"
#include "imgui_log_panel.hpp"

namespace physicat::graphics {
    struct ImGuiRenderer {
        ImGuiRenderer(SDL_Window* window, SDL_GLContext& context);
        ~ImGuiRenderer();

        void Input(const SDL_Event& event);
        void Render(physicat::Scene& scene, unsigned int frameBufferId, const double fps);

        // Closes any child processes like tracy
        void ClosePIDs();

//        bool IsSceneViewportFocused() const;
//        const WindowSize& GetSceneViewportSize() const;

    private:
        void OpenTracyProfiler();

        void CreateNewFrame();
        void DrawFrame(physicat::Scene& scene, uint32_t frameBufferId, const double fps);
        void RenderFrame();

        void CreateDockingSpace();

//        void CreateRender3DPanel(unsigned int frameBufferId);
//        bool isSceneViewportFocused; // soon come up with good naming conventions
//        WindowSize SceneViewportSize;

//        void CreateLifeObjectSelectorPanel(physicat::Scene& scene);
//        void CreateSelectableItem(bool isEnd);
//        const ImGuiTreeNodeFlags SelectableFlags;

//        void CreateObjectEditorPanel(const core::LifeObject& selectedLifeObject);

//        void CreateLogPanel();

        bool IsRendering;
        physicat::graphics::ui::ImGuiStructurePanel StructurePanel;
        physicat::graphics::ui::ImGuiEditPanel EditPanel;
        physicat::editor::ImGuiWorldRenderPanel WorldRenderPanel;
        physicat::editor::ImGuiLogPanel LogPanel;
    };
}


#endif //PHYSICAT_IMGUI_RENDERER_HPP
