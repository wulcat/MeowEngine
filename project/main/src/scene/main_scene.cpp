//
// Created by Akira Mujawar on 21/06/24.
//

#include "main_scene.hpp"

#include "perspective_camera.hpp"
#include "static_mesh_instance.hpp"
#include "log.hpp"
#include "opengl_line_pipeline.hpp"

#include "src/core/assets/lifeobject/life_object.hpp"
#include "mesh_render_component.hpp"
#include "line_render_component.hpp"
#include "transform3d_component.hpp"

using physicat::MainScene;
using physicat::assets::ShaderPipelineType;
using physicat::assets::StaticMeshType;
using physicat::assets::TextureType;
using physicat::core::component::RenderComponentBase;
using physicat::core::component::MeshRenderComponent;
using physicat::core::component::LineRenderComponent;
using physicat::core::component::Transform3DComponent;

namespace {
    physicat::PerspectiveCamera CreateCamera(const float& width, const float& height) {
        return physicat::PerspectiveCamera(width, height);
    }
}

struct MainScene::Internal {
    physicat::PerspectiveCamera Camera;
    std::vector<core::LifeObject> LifeObjects;
//    core::LifeObject meshObject;
//    core::LifeObject meshObject1;
//    core::LifeObject lineObject1;

    Internal(const float& screenWidth, const float& screenHeight)
        : Camera(::CreateCamera(screenWidth, screenHeight)) {}

    void Create(physicat::AssetManager& assetManager) {
        assetManager.LoadShaderPipelines({
            ShaderPipelineType::Grid,
            ShaderPipelineType::Default,
            ShaderPipelineType::Line
        });

        assetManager.LoadStaticMeshes({
            StaticMeshType::Plane,
            StaticMeshType::Cube,
            StaticMeshType::Sphere,
            StaticMeshType::Cylinder,
            StaticMeshType::Cone,
            StaticMeshType::Torus
        });

        assetManager.LoadTextures({
            TextureType::Default,
            TextureType::Pattern
        });

//        StaticMeshes.push_back(
//                physicat::StaticMeshInstance{
//                        StaticMeshType::Plane,
//                        TextureType::Default,
//                        glm::vec3{0.0f, -0.3f, 0.0f},
//                        glm::vec3{0.6f, 0.6f, 0.6f},
//                        glm::vec3{0.0f, 1.0f, 0},
//                        0.0f
//                }
//        );
//
//        StaticMeshes.push_back(
//                physicat::StaticMeshInstance{
//                        StaticMeshType::Cube,
//                        TextureType::Pattern,
//                        glm::vec3{1.0f, -1.3f, -2.0f},
//                        glm::vec3{0.6f, 0.6f, 0.6f},
//                        glm::vec3{0.0f, 0.4f, 0.9f},
//                        0.0f
//                }
//        );
//
//        StaticMeshes.push_back(
//                physicat::StaticMeshInstance{
//                        StaticMeshType::Sphere,
//                        TextureType::Pattern,
//                        glm::vec3{-2.4f, -0.6f, -3.0f},
//                        glm::vec3{0.6f, 0.6f, 0.6f},
//                        glm::vec3{0.0f, 0.4f, 0.9f},
//                        0.0f
//                }
//        );
//
//        StaticMeshes.push_back(
//                physicat::StaticMeshInstance{
//                        StaticMeshType::Cylinder,
//                        TextureType::Pattern,
//                        glm::vec3{2.1f, 0.7f, -3.0f},
//                        glm::vec3{0.6f, 0.6f, 0.6f},
//                        glm::vec3{0.0f, 0.4f, 0.9f},
//                        0.0f
//                }
//        );
//
//        StaticMeshes.push_back(
//                physicat::StaticMeshInstance{
//                        StaticMeshType::Cone,
//                        TextureType::Pattern,
//                        glm::vec3{-1.4f, 1.2f, -2.5f},
//                        glm::vec3{0.6f, 0.6f, 0.6f},
//                        glm::vec3{0.0f, 0.4f, 0.9f},
//                        0.0f
//                }
//        );
//
//        StaticMeshes.push_back(
//            physicat::StaticMeshInstance{
//                StaticMeshType::Torus,
//                TextureType::Pattern,
//                glm::vec3{-0.3f, 0.4f, -2.0f},
//                glm::vec3{0.4f, 0.4f, 0.4f},
//                glm::vec3{0.6f, 0.3f, 0.1f},
//                0.0f
//            }
//        );


        core::LifeObject meshObject = core::LifeObject();
        meshObject.SetTransformComponent(
            new Transform3DComponent(
                glm::vec3{0.0f, 0.0f, 2},
                glm::vec3{0.5f, 0.5f,0.5f},
                glm::vec3{0.0f, 1.0f, 0.0f},
                0
            )
        );
        meshObject.SetRenderComponent(
            new MeshRenderComponent(
                physicat::assets::ShaderPipelineType::Default,
                new physicat::StaticMeshInstance{
                    StaticMeshType::Cube,
                    TextureType::Pattern
                }
            )
        );

        core::LifeObject meshObject1 = core::LifeObject();
        meshObject1.SetTransformComponent(
            new Transform3DComponent(
                glm::vec3{-1.4f, -0.5f, 0},
                glm::vec3{0.6f, 0.6f, 0.6f},
                glm::vec3{0.0f, 0.4f, 0.9f},
                0.0f
            )
        );
        meshObject1.SetRenderComponent(
            new MeshRenderComponent(
                physicat::assets::ShaderPipelineType::Default,
                new physicat::StaticMeshInstance{
                    StaticMeshType::Cone,
                    TextureType::Pattern
                }
            )
        );
        core::LifeObject meshObject2 = core::LifeObject();
        meshObject2.SetTransformComponent(
                new Transform3DComponent(
                        glm::vec3{1.0f, 0.0f, 0},
                        glm::vec3{1.0f, 1, 1.0f},
                        glm::vec3{0.0f, 0.4f, 0.9f},
                        0.0f
                )
        );
        meshObject2.SetRenderComponent(
            new MeshRenderComponent(
                physicat::assets::ShaderPipelineType::Default,
                new physicat::StaticMeshInstance{
                    StaticMeshType::Sphere,
                    TextureType::Pattern
                }
            )
        );

        core::LifeObject gridObject = core::LifeObject();
        gridObject.SetTransformComponent(
            new Transform3DComponent(
                glm::vec3{0, 0, 0},
                glm::vec3{1.0, 1.0f, 1.0f},
                glm::vec3{0.0f, 1.0f, 0.0f},
                0.0f
            )
        );
        gridObject.SetRenderComponent(
            new RenderComponentBase(physicat::assets::ShaderPipelineType::Grid)
        );

        // NOTE: The order of rendering matters when depth and transparency is involved
        // It's not a topic i want to explore but if in future needed check out Painter's algorithm and z-buffer
        LifeObjects.push_back(meshObject);
        LifeObjects.push_back(meshObject1);
        LifeObjects.push_back(meshObject2);

        LifeObjects.push_back(gridObject);
    }

    // We can perform -> culling, input detection
    void Update(const float& deltaTime) {
        const glm::mat4 cameraMatrix {Camera.GetProjectionMatrix() * Camera.GetViewMatrix()};

        for(auto& lifeObject : LifeObjects) {
            lifeObject.TransformComponent->Update(cameraMatrix);
        }
    }

    void Render(physicat::Renderer& renderer) {
        // This is important for now - we can come to this later for optimization
        // Current goal is to have full control on render as individual objects
        // as we will have elements like UI, Static Meshes, Post Processing, Camera Culling, Editor Tools
        for(auto& lifeObject : LifeObjects) {
            renderer.Render(&Camera, &lifeObject);
        }
    }
};

MainScene::MainScene(const float &screenWidth, const float &screenHeight)
    : InternalPointer(physicat::make_internal_ptr<Internal>(screenWidth, screenHeight)){}

void MainScene::Create(physicat::AssetManager &assetManager) {
    InternalPointer->Create(assetManager);
}

void MainScene::Update(const float &deltaTime) {
    InternalPointer->Update(deltaTime);
}

void MainScene::Render(physicat::Renderer &renderer) {
    InternalPointer->Render(renderer);
}












