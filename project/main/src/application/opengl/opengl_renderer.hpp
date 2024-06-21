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
        OpenGLRenderer(std::shared_ptr<physicat::OpenGLAssetManager> assetManager);

        void Render(
            const physicat::assets::ShaderPipelineType& shaderPipelineType,
            const std::vector<physicat::StaticMeshInstance>& staticMeshInstances
        ) override;

    private:
        struct Internal;
        physicat::internal_ptr<Internal> InternalPointer;
    };
}


#endif //PHYSICAT_OPENGL_RENDERER_HPP
