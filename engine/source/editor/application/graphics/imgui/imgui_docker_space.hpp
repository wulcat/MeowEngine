//
// Created by Akira Mujawar on 14/07/24.
//

#ifndef PHYSICAT_IMGUI_DOCKER_SPACE_HPP
#define PHYSICAT_IMGUI_DOCKER_SPACE_HPP

namespace physicat::editor {
    struct ImGuiDockerSpace {
        ImGuiDockerSpace();

        ~ImGuiDockerSpace();

    private:
        bool IsActive;
    };
}


#endif //PHYSICAT_IMGUI_DOCKER_SPACE_HPP
