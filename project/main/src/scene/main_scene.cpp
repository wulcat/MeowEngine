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
                glm::vec3{-0.3f, 0.4f, 0},
                glm::vec3{1, 1,1},
                glm::vec3{0.6f, 0.3f, 0.1f},
                0
            )
        );
        meshObject.SetRenderComponent(
            new MeshRenderComponent(
                physicat::assets::ShaderPipelineType::Default,
                new physicat::StaticMeshInstance{
                    StaticMeshType::Torus,
                    TextureType::Pattern
                }
            )
        );

//        core::LifeObject meshObject1 = core::LifeObject();
//        meshObject1.SetTransformComponent(
//            new Transform3DComponent(
//                glm::vec3{-1.4f, 1.2f, 0},
//                glm::vec3{0.6f, 0.6f, 0.6f},
//                glm::vec3{0.0f, 0.4f, 0.9f},
//                0.0f
//            )
//        );
//        meshObject1.SetRenderComponent(
//            new MeshRenderComponent(
//                physicat::assets::ShaderPipelineType::Default,
//                new physicat::StaticMeshInstance{
//                    StaticMeshType::Cone,
//                    TextureType::Pattern
//                }
//            )
//        );
        core::LifeObject meshObject1 = core::LifeObject();
        meshObject1.SetTransformComponent(
                new Transform3DComponent(
                        glm::vec3{0, -1, 0},
                        glm::vec3{10.0f, 1, 10.0f},
                        glm::vec3{0.0f, 0.4f, 0.9f},
                        0.0f
                )
        );
        meshObject1.SetRenderComponent(
                new MeshRenderComponent(
                        physicat::assets::ShaderPipelineType::Default,
                        new physicat::StaticMeshInstance{
                                StaticMeshType::Plane,
                                TextureType::Pattern
                        }
                )
        );

        LifeObjects.push_back(meshObject);
//        LifeObjects.push_back(meshObject1);

        for(int i = -20; i < 20; i++) {
            core::LifeObject lineObject1 = core::LifeObject();
            lineObject1.SetTransformComponent(
                new Transform3DComponent(
                    glm::vec3{0, 0, i},
                    glm::vec3{1, 1, 1},
                    glm::vec3{0.0f, 1.0f, 0.0f},
                    0.0f
                )
            );

            lineObject1.SetRenderComponent(
                new LineRenderComponent(
                    physicat::assets::ShaderPipelineType::Line,
                    glm::vec3(-1000, 0, 0),
                    glm::vec3(1000, 0, 0)
                )
            );

            LifeObjects.push_back(lineObject1);
        }

        for(int i = -20; i < 20; i++) {
            core::LifeObject lineObject1 = core::LifeObject();
            lineObject1.SetTransformComponent(
                    new Transform3DComponent(
                            glm::vec3{i, 0, 0},
                            glm::vec3{1, 1, 1},
                            glm::vec3{0.0f, 1.0f, 0.0f},
                            0.0f
                    )
            );

            lineObject1.SetRenderComponent(
                    new LineRenderComponent(
                            physicat::assets::ShaderPipelineType::Line,
                            glm::vec3(0, 0, -1000),
                            glm::vec3(0, 0, 1000)
                    )
            );

            LifeObjects.push_back(lineObject1);
        }
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












