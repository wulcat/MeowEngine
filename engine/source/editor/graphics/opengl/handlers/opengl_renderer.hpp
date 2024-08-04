//
// Created by Akira Mujawar on 21/06/24.
//

#ifndef PHYSICAT_OPENGL_RENDERER_HPP
#define PHYSICAT_OPENGL_RENDERER_HPP

#include "internal_ptr.hpp"
#include "renderer.hpp"

#include "opengl_asset_manager.hpp"


#include <memory>

namespace physicat {
    struct OpenGLRenderer : public physicat::Renderer {
        OpenGLRenderer(const std::shared_ptr<physicat::OpenGLAssetManager>& assetManager);

//        void Render(physicat::PerspectiveCamera* cameraObject, physicat::core::LifeObject* lifeObject) override;
        void Render(physicat::PerspectiveCamera* cameraObject, entt::registry& registry) override;

    private:
        struct Internal;
        physicat::internal_ptr<Internal> InternalPointer;
    };
}


#endif //PHYSICAT_OPENGL_RENDERER_HPP
