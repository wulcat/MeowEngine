//
// Created by Akira Mujawar on 08/07/24.
//

#ifndef PHYSICAT_IMGUI_RENDERER_HPP
#define PHYSICAT_IMGUI_RENDERER_HPP

namespace physicat::graphics {
    struct ImGuiRenderer {
        ImGuiRenderer(SDL_Window* window, SDL_GLContext& context);
        ~ImGuiRenderer();

        void Input(const SDL_Event& event);
        void Update();
        void Render();

        // Closes any child processes like tracy
        void ClosePIDs();

    private:
        void OpenTracyProfiler();

    };
}


#endif //PHYSICAT_IMGUI_RENDERER_HPP
