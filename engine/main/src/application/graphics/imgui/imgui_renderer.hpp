//
// Created by Akira Mujawar on 08/07/24.
//

#ifndef PHYSICAT_IMGUI_RENDERER_HPP
#define PHYSICAT_IMGUI_RENDERER_HPP

#include "opengl_framebuffer.hpp"
#include "window_size.hpp"

namespace physicat::graphics {
    struct ImGuiRenderer {
        ImGuiRenderer(SDL_Window* window, SDL_GLContext& context);
        ~ImGuiRenderer();

        void Input(const SDL_Event& event);
        void Render(unsigned int frameBufferId);

        // Closes any child processes like tracy
        void ClosePIDs();

        bool IsSceneViewportFocused() const;
        const WindowSize& GetSceneViewportSize() const;

    private:
        void OpenTracyProfiler();

        void CreateNewFrame();
        void DrawFrame(unsigned int frameBufferId);
        void RenderFrame();

        bool isSceneViewportFocused;
        WindowSize SceneViewportSize;
    };
}


#endif //PHYSICAT_IMGUI_RENDERER_HPP
