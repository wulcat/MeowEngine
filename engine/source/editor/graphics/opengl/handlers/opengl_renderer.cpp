//
// Created by Akira Mujawar on 21/06/24.
//

#include "opengl_renderer.hpp"


#include "opengl_mesh_pipeline.hpp"
#include "opengl_line_pipeline.hpp"
#include "opengl_grid_pipeline.hpp"


using physicat::OpenGLRenderer;

using namespace physicat::pipeline;
using namespace physicat::core::component;
using physicat::assets::ShaderPipelineType;

struct OpenGLRenderer::Internal {
    const std::shared_ptr<physicat::OpenGLAssetManager> AssetManager;

    Internal(std::shared_ptr<physicat::OpenGLAssetManager> assetManager)
    : AssetManager(assetManager){}

//    void Render(physicat::PerspectiveCamera* cameraObject, physicat::core::LifeObject* lifeObject) {
//
////        AssetManager->GetShaderPipeline(shaderPipelineType).Render(
////                *AssetManager,
////                staticMeshInstances,
////                lineDraw
////        );
//
//        RenderComponentBase* renderComponent = lifeObject->RenderComponent;
//
//        switch (renderComponent->GetShaderPipelineType()) {
//            case ShaderPipelineType::Default:
////                AssetManager->GetShaderPipeline<OpenGLMeshPipeline>(ShaderPipelineType::Default)->Render(
////                    *AssetManager,
////                    dynamic_cast<MeshRenderComponent*>(renderComponent),
////                    dynamic_cast<physicat::entity::Transform3dComponent*>(lifeObject->TransformComponent)
////                );
//                break;
//            case ShaderPipelineType::Line:
//                AssetManager->GetShaderPipeline<OpenGLLinePipeline>(ShaderPipelineType::Line)->Render(
//                    *AssetManager,
//                    dynamic_cast<LineRenderComponent*>(renderComponent),
//                    dynamic_cast<physicat::core::component::Transform3DComponent*>(lifeObject->TransformComponent),
//                    cameraObject
//                );
//                break;
//            case ShaderPipelineType::Grid:
//                AssetManager->GetShaderPipeline<OpenGLGridPipeline>(ShaderPipelineType::Grid)->Render(
//                        *AssetManager,
//                        renderComponent,
//                        dynamic_cast<physicat::core::component::Transform3DComponent*>(lifeObject->TransformComponent),
//                        cameraObject
//                );
//                break;
//        }
//    }

    void Render(physicat::PerspectiveCamera* cameraObject, entt::registry& registry)
    {
//        auto view = registry.view<physicat::core::component::Transform3DComponent>();
//        for(auto entity: view)
//        {
//            auto transform = view.get<physicat::core::component::Transform3DComponent>(entity);
//
//        }

//        auto view = registry.view<entity::Transform3dComponent>();
//        registry.view<physicat::entity::Transform3dComponent>().each([](auto entity, auto &physicat::entity::Transform3dComponent) {
//            // ...
//        });
//
//        for(auto &&[entt::entity, physicat::entity::Transform3dComponent]: registry.view<entity::Transform3dComponent>().each()) {
//            // ...
//        }
        for(auto &&[entity,renderComponent, transform]: registry.view<physicat::core::component::MeshRenderComponent, physicat::core::component::Transform3DComponent>().each())
        {
            AssetManager->GetShaderPipeline<OpenGLMeshPipeline>(ShaderPipelineType::Default)->Render(
                    *AssetManager,
                    &renderComponent,
                    &transform
            );
        }

        for(auto &&[entity,renderComponent, transform]: registry.view<physicat::core::component::RenderComponentBase, physicat::core::component::Transform3DComponent>().each())
        {
            AssetManager->GetShaderPipeline<OpenGLGridPipeline>(ShaderPipelineType::Grid)->Render(
                    *AssetManager,
                    &renderComponent,
                    &transform,
                    cameraObject
            );
        }
    }
};

OpenGLRenderer::OpenGLRenderer(const std::shared_ptr<physicat::OpenGLAssetManager>& assetManager)
    : InternalPointer(physicat::make_internal_ptr<Internal>(assetManager)) {}

//void OpenGLRenderer::Render(physicat::PerspectiveCamera* cameraObject, physicat::core::LifeObject* lifeObject) {
//    InternalPointer->Render(cameraObject, lifeObject);
//}

void OpenGLRenderer::Render(physicat::PerspectiveCamera* cameraObject, entt::registry& registry)
{
    InternalPointer->Render(cameraObject, registry);
}
