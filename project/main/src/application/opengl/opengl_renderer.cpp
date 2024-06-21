//
// Created by Akira Mujawar on 21/06/24.
//

#include "opengl_renderer.hpp"

using physicat::OpenGLRenderer;

struct OpenGLRenderer::Internal {
    const std::shared_ptr<physicat::OpenGLAssetManager> AssetManager;

    Internal(std::shared_ptr<physicat::OpenGLAssetManager> assetManager)
    : AssetManager(assetManager){}

    void Render(
        const physicat::assets::ShaderPipelineType &shaderPipelineType,
        const std::vector<physicat::StaticMeshInstance> &staticMeshInstances) {
        AssetManager->GetShaderPipeline(shaderPipelineType).Render(*AssetManager, staticMeshInstances);
    }
};

OpenGLRenderer::OpenGLRenderer(std::shared_ptr<physicat::OpenGLAssetManager> assetManager)
    : InternalPointer(physicat::make_internal_ptr<Internal>(assetManager)) {}

void OpenGLRenderer::Render(
    const physicat::assets::ShaderPipelineType &shaderPipelineType,
    const std::vector<physicat::StaticMeshInstance> &staticMeshInstances) {
    InternalPointer->Render(shaderPipelineType, staticMeshInstances);
}
