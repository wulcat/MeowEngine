//
// Created by Akira Mujawar on 23/06/24.
//

#ifndef MEOWENGINE_OPENGL_MESH_PIPELINE_HPP
#define MEOWENGINE_OPENGL_MESH_PIPELINE_HPP

#include "glm_wrapper.hpp"
#include "graphics_wrapper.hpp"

#include "opengl_pipeline_base.hpp"
#include "mesh_render_component.hpp"
#include "transform3d_component.hpp"

namespace MeowEngine::pipeline {
    struct OpenGLMeshPipeline : public MeowEngine::pipeline::OpenGLPipelineBase {
        OpenGLMeshPipeline(const GLuint& shaderProgramID);
        ~OpenGLMeshPipeline() override;

    public:
        void Render(
            const MeowEngine::OpenGLAssetManager& assetManager,
            const MeowEngine::entity::MeshRenderComponent* meshRenderComponent,
            const MeowEngine::entity::Transform3DComponent* transform3DComponent
        ) const;
//        void Render(
//                const MeowEngine::OpenGLAssetManager& assetManager,
//                const MeowEngine::entity::StaticMeshRenderComponent* meshRenderComponent,
//                const MeowEngine::entity::Transform3dComponent* transform3DComponent
//        ) const;

    private:
        const GLuint ShaderProgramID;
        const GLuint UniformLocationMVP;
        const GLuint AttributeLocationVertexPosition;
        const GLuint AttributeLocationTextureCoord;
        const GLsizei Stride;
        const GLsizei OffsetPosition;
        const GLsizei OffsetTextureCoord;
    };
}


#endif //MEOWENGINE_OPENGL_MESH_PIPELINE_HPP
