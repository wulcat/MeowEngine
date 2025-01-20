//
// Created by Akira Mujawar on 14/07/24.
//

#ifndef MEOWENGINE_IMGUI_DOCKER_SPACE_HPP
#define MEOWENGINE_IMGUI_DOCKER_SPACE_HPP

namespace MeowEngine::editor {
    struct ImGuiDockerSpace {
        ImGuiDockerSpace();

        ~ImGuiDockerSpace();

    private:
        bool IsActive;
    };
}


#endif //MEOWENGINE_IMGUI_DOCKER_SPACE_HPP
