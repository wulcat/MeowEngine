//
// Created by Akira Mujawar on 21/06/24.
//

#include "main_scene.hpp"

#include "camera_controller.hpp"
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
    physicat::PerspectiveCamera CreateCamera(const physicat::WindowSize& size) {
        return physicat::PerspectiveCamera(static_cast<float>(size.width), static_cast<float>(size.height));
    }
}

struct MainScene::Internal {
    physicat::PerspectiveCamera Camera;
    physicat::CameraController CameraController;
    std::vector<core::LifeObject> LifeObjects;

    // User Input Events
    const uint8_t* KeyboardState; // SDL owns the object & will manage the lifecycle. We just keep a pointer.

    Internal(const physicat::WindowSize& size)
        : Camera(::CreateCamera(size))
        , CameraController({glm::vec3(0.0f, 2.0f , -10.0f)})
        , KeyboardState(SDL_GetKeyboardState(nullptr))
    {}

    void OnWindowResized(const physicat::WindowSize& size) {
        Camera = ::CreateCamera(size);
    }

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
                        glm::vec3{0.5f, 0.4f, 0.9f},
                        0.0f
                )
        );
        meshObject2.SetRenderComponent(
            new MeshRenderComponent(
                physicat::assets::ShaderPipelineType::Default,
                new physicat::StaticMeshInstance{
                    StaticMeshType::Torus,
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
        // Not a topic i want to explore but if in future needed check out Painter's algorithm and z-buffer
        LifeObjects.push_back(meshObject);
        LifeObjects.push_back(meshObject1);
        LifeObjects.push_back(meshObject2);

        LifeObjects.push_back(gridObject);
    }

    void ProcessInput(const float& delta) {
        int deltaMouseX;
        int deltaMouseY;
        uint32_t mouseState = SDL_GetRelativeMouseState(&deltaMouseX, &deltaMouseY);

        if(mouseState & SDL_BUTTON(0)) {
            CameraController.LookAround(deltaMouseX, deltaMouseY);
        }


        if (KeyboardState[SDL_SCANCODE_UP] || KeyboardState[SDL_SCANCODE_W]) {
            CameraController.MoveForward(delta);
        }

        if (KeyboardState[SDL_SCANCODE_DOWN] || KeyboardState[SDL_SCANCODE_S]) {
            CameraController.MoveBackward(delta);
        }

        if (KeyboardState[SDL_SCANCODE_Q] ) {
            CameraController.MoveUp(delta);
        }

        if (KeyboardState[SDL_SCANCODE_E]) {
            CameraController.MoveDown(delta);
        }

//        if (KeyboardState[SDL_SCANCODE_LEFT] || KeyboardState[SDL_SCANCODE_A]) {
//            CameraController.TurnLeft(delta);
//        }
//
//        if (KeyboardState[SDL_SCANCODE_RIGHT] || KeyboardState[SDL_SCANCODE_D]) {
//            CameraController.TurnRight(delta);
//        }
    }

    // We can perform -> culling, input detection
    void Update(const float& deltaTime) {
        ProcessInput(deltaTime);

        Camera.Configure(CameraController.GetPosition(), CameraController.GetUp(), CameraController.GetDirection());

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

MainScene::MainScene(const physicat::WindowSize& size)
    : InternalPointer(physicat::make_internal_ptr<Internal>(size)){}

void MainScene::OnWindowResized(const physicat::WindowSize &size) {
    InternalPointer->OnWindowResized(size);
}

void MainScene::Create(physicat::AssetManager &assetManager) {
    InternalPointer->Create(assetManager);
}

void MainScene::Update(const float &deltaTime) {
    InternalPointer->Update(deltaTime);
}

void MainScene::Render(physicat::Renderer &renderer) {
    InternalPointer->Render(renderer);
}












