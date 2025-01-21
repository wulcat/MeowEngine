//
// Created by Akira Mujawar on 21/06/24.
//

#ifndef MEOWENGINE_OPENGL_RENDER_SYSTEM_HPP
#define MEOWENGINE_OPENGL_RENDER_SYSTEM_HPP

#include "internal_ptr.hpp"
#include "render_system.hpp"

#include "opengl_asset_manager.hpp"
#include "imgui_renderer.hpp"

#include <memory>

namespace MeowEngine {
    struct OpenGLRenderer : public MeowEngine::Renderer {
        OpenGLRenderer(const std::shared_ptr<MeowEngine::OpenGLAssetManager>& assetManager,
                       const std::shared_ptr<MeowEngine::graphics::ImGuiRenderer>& uiRenderer);

        void RenderGameView(MeowEngine::PerspectiveCamera* cameraObject, entt::registry& registry) override;
        void RenderUserInterface(entt::registry& registry, std::queue<std::shared_ptr<MeowEngine::ReflectionPropertyChange>>& inUIInputQueue, unsigned int frameBufferId, const double fps) override;

    private:
        struct Internal;
        MeowEngine::internal_ptr<Internal> InternalPointer;
    };
}


#endif //MEOWENGINE_OPENGL_RENDER_SYSTEM_HPP
