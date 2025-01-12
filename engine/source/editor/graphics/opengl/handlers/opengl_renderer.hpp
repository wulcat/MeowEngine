//
// Created by Akira Mujawar on 21/06/24.
//

#ifndef PHYSICAT_OPENGL_RENDERER_HPP
#define PHYSICAT_OPENGL_RENDERER_HPP

#include "internal_ptr.hpp"
#include "renderer.hpp"

#include "opengl_asset_manager.hpp"
#include "imgui_renderer.hpp"

#include <memory>

namespace physicat {
    struct OpenGLRenderer : public physicat::Renderer {
        OpenGLRenderer(const std::shared_ptr<physicat::OpenGLAssetManager>& assetManager,
                       const std::shared_ptr<physicat::graphics::ImGuiRenderer>& uiRenderer);

        void Render(physicat::PerspectiveCamera* cameraObject, entt::registry& registry) override;
        void RenderUI(entt::registry& registry, std::queue<physicat::ReflectionPropertyChange>& inUIInputQueue, unsigned int frameBufferId, const double fps) override;

    private:
        struct Internal;
        physicat::internal_ptr<Internal> InternalPointer;
    };
}


#endif //PHYSICAT_OPENGL_RENDERER_HPP
