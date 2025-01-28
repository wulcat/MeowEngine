//
// Created by Akira Mujawar on 27/01/25.
//

#include "scene_single_thread.hpp"

#include "camera_controller.hpp"
#include "perspective_camera.hpp"
#include "static_mesh_instance.hpp"

#include "mesh_render_component.hpp"
#include "line_render_component.hpp"
#include "transform2d_component.hpp"
#include "transform3d_component.hpp"

#include "life_object_component.hpp"
#include "transform3d_component.hpp"
#include "box_collider_data.hpp"
#include "collider_component.hpp"

#include "sky_box_component.hpp"

#include "rigidbody_component.hpp"
#include "entt_reflection_wrapper.hpp"
#include "entt_single_buffer.hpp"

namespace {
    MeowEngine::PerspectiveCamera CreateCamera(const MeowEngine::WindowSize& size) {
        return MeowEngine::PerspectiveCamera(static_cast<float>(size.Width), static_cast<float>(size.Height));
    }
}

namespace MeowEngine {
    struct SceneSingleThread::Internal {
        MeowEngine::PerspectiveCamera Camera;
        MeowEngine::CameraController CameraController;

        MeowEngine::EnttSingleBuffer RegistryBuffer;

        // User Input Events
        const uint8_t* KeyboardState; // SDL owns the object & will manage the lifecycle. We just keep a pointer.

        Internal(const MeowEngine::WindowSize& size)
        : Camera(::CreateCamera(size))
        , CameraController({glm::vec3(0.0f, 2.0f , -10.0f)})
        , KeyboardState(SDL_GetKeyboardState(nullptr))
        , RegistryBuffer() {}

        void OnWindowResized(const MeowEngine::WindowSize& size) {
            Camera = ::CreateCamera(size);
        }

        void Load(std::shared_ptr<MeowEngine::AssetManager> assetManager) {
            assetManager->LoadShaderPipelines({
                                                      assets::ShaderPipelineType::Grid,
                                                      assets::ShaderPipelineType::Default,
                                                      assets::ShaderPipelineType::Line,
                                                      assets::ShaderPipelineType::Sky
                                              });

            assetManager->LoadStaticMeshes({
                                                   assets::StaticMeshType::Plane,
                                                   assets::StaticMeshType::Cube,
                                                   assets::StaticMeshType::Sphere,
                                                   assets::StaticMeshType::Cylinder,
                                                   assets::StaticMeshType::Cone,
                                                   assets::StaticMeshType::Torus
                                           });

            assetManager->LoadTextures({
                                               assets::TextureType::Default,
                                               assets::TextureType::Pattern
                                       });

            REGISTER_ENTT_COMPONENT(LifeObjectComponent);

            REGISTER_ENTT_COMPONENT(Transform2DComponent);
            REGISTER_ENTT_COMPONENT(Transform3DComponent);

            REGISTER_ENTT_COMPONENT(ColliderComponent);
            REGISTER_ENTT_COMPONENT(RigidbodyComponent);

            REGISTER_ENTT_COMPONENT(RenderComponentBase);
            REGISTER_ENTT_COMPONENT(LineRenderComponent);
            REGISTER_ENTT_COMPONENT(MeshRenderComponent);
            REGISTER_ENTT_COMPONENT(SkyBoxComponent);
        }


        void Create(MeowEngine::simulator::PhysicsSystem* inPhysics) {
            auto entity = RegistryBuffer.AddEntity();
            RegistryBuffer.AddComponent<entity::LifeObjectComponent>(entity, "torus");
            RegistryBuffer.AddComponent<entity::Transform3DComponent>(
                    entity,
                    Camera.GetProjectionMatrix() * Camera.GetViewMatrix(),
                    glm::vec3{5, 0, 0},
                    glm::vec3{1.0, 1.0f, 1.0f},
                    glm::vec3{0.0f, 1.0f, 0.0f},
                    12.0f
            );
            RegistryBuffer.AddComponent<entity::MeshRenderComponent>(
                    entity,
                    assets::ShaderPipelineType::Default,
                    new MeowEngine::StaticMeshInstance{
                            assets::StaticMeshType::Torus,
                            assets::TextureType::Pattern
                    }
            );

            const auto cubeEntity = RegistryBuffer.AddEntity();
            RegistryBuffer.AddComponent<entity::LifeObjectComponent>(cubeEntity, "cube");
            RegistryBuffer.AddComponent<entity::Transform3DComponent>(
                    cubeEntity,
                    Camera.GetProjectionMatrix() * Camera.GetViewMatrix(),
                    glm::vec3{0.0f, 20.0f, 2},
                    glm::vec3{0.5f, 0.5f,0.5f},
                    glm::vec3{0.0f, 1.0f, 0.0f},
                    0
            );
            RegistryBuffer.AddComponent<entity::MeshRenderComponent>(
                    cubeEntity,
                    assets::ShaderPipelineType::Default,
                    new MeowEngine::StaticMeshInstance{
                            assets::StaticMeshType::Cube,
                            assets::TextureType::Pattern
                    }
            );
            RegistryBuffer.AddComponent<entity::ColliderComponent>(
                    cubeEntity,
                    entity::ColliderType::BOX,
                    new entity::BoxColliderData()
            );
            RegistryBuffer.AddComponent<entity::RigidbodyComponent>(
                    cubeEntity
            );

            const auto cubeEntity1 = RegistryBuffer.AddEntity();
            RegistryBuffer.AddComponent<entity::LifeObjectComponent>(cubeEntity1, "cube1");
            RegistryBuffer.AddComponent<entity::Transform3DComponent>(
                    cubeEntity1,
                    Camera.GetProjectionMatrix() * Camera.GetViewMatrix(),
                    glm::vec3{0.0f, 0.0f, 2},
                    glm::vec3{0.5f, 0.5f,0.5f},
                    glm::vec3{0.0f, 1.0f, 0.0f},
                    0
            );
            RegistryBuffer.AddComponent<entity::MeshRenderComponent>(
                    cubeEntity1,
                    assets::ShaderPipelineType::Default,
                    new MeowEngine::StaticMeshInstance{
                            assets::StaticMeshType::Cube,
                            assets::TextureType::Pattern
                    }
            );
            RegistryBuffer.AddComponent<entity::ColliderComponent>(
                    cubeEntity1,
                    entity::ColliderType::BOX,
                    new entity::BoxColliderData()
            );
            RegistryBuffer.AddComponent<entity::RigidbodyComponent>(
                    cubeEntity1
            );

            for(int i = 0 ; i < 2500; i++){
                const auto cubeTest = RegistryBuffer.AddEntity();
                RegistryBuffer.AddComponent<entity::LifeObjectComponent>(cubeTest, "cube");
                RegistryBuffer.AddComponent<entity::Transform3DComponent>(
                        cubeTest,
                        Camera.GetProjectionMatrix() * Camera.GetViewMatrix(),
                        glm::vec3{0.0f, 20.0f + i, 2},
                        glm::vec3{0.5f, 0.5f,0.5f},
                        glm::vec3{0.0f, 1.0f, 0.0f},
                        0
                );
                RegistryBuffer.AddComponent<entity::MeshRenderComponent>(
                        cubeTest,
                        assets::ShaderPipelineType::Default,
                        new MeowEngine::StaticMeshInstance{
                                assets::StaticMeshType::Cube,
                                assets::TextureType::Pattern
                        }
                );
                RegistryBuffer.AddComponent<entity::ColliderComponent>(
                        cubeTest,
                        entity::ColliderType::BOX,
                        new entity::BoxColliderData()
                );
                RegistryBuffer.AddComponent<entity::RigidbodyComponent>(
                        cubeTest
                );
            }

            // setup object
            // later query for all rigidbody, get the physx, get the collider and construct for physics

            const auto gridEntity = RegistryBuffer.AddEntity();
            RegistryBuffer.AddComponent<entity::LifeObjectComponent>(gridEntity, "grid");
            RegistryBuffer.AddComponent<entity::Transform3DComponent>(
                    gridEntity,
                    Camera.GetProjectionMatrix() * Camera.GetViewMatrix(),
                    glm::vec3{0, 0, 0},
                    glm::vec3{1.0, 1.0f, 1.0f},
                    glm::vec3{0.0f, 1.0f, 0.0f},
                    0.0f
            );
            RegistryBuffer.AddComponent<entity::RenderComponentBase>(
                    gridEntity,
                    assets::ShaderPipelineType::Grid
            );

            const auto skyEntity = RegistryBuffer.AddEntity();
            RegistryBuffer.AddComponent<entity::LifeObjectComponent>(skyEntity, "sky");
            RegistryBuffer.AddComponent<entity::Transform3DComponent>(
                    skyEntity,
                    Camera.GetProjectionMatrix() * Camera.GetViewMatrix(),
                    glm::vec3{0, 0, 0},
                    glm::vec3{1.0, 1.0f, 1.0f},
                    glm::vec3{0.0f, 1.0f, 0.0f},
                    0.0f
            );
            RegistryBuffer.AddComponent<entity::SkyBoxComponent>(
                    skyEntity,
                    assets::ShaderPipelineType::Sky
            );

            // Creating physics for all bodies in game
            for(auto &&[entity, transform, collider, rigidbody]
                    : RegistryBuffer.GetCurrent().view<entity::Transform3DComponent, entity::ColliderComponent, entity::RigidbodyComponent>().each()) {
                inPhysics->AddRigidbody(transform, collider, rigidbody);
            }

            MeowEngine::Log("Creating", "Created");
        }

        void Input(const float& delta, const MeowEngine::input::InputManager& inputManager) {
            if(!inputManager.isActive) {
                return;
            }

            if(inputManager.mouseState & SDL_BUTTON_LMASK) {
                CameraController.LookAround(inputManager.mouseDeltaX, inputManager.mouseDeltaY);
            }

            if(inputManager.isMouseDown && (inputManager.mouseState & SDL_BUTTON_RMASK)) {
                const auto cubeEntity = RegistryBuffer.AddEntity();
                RegistryBuffer.AddComponent<entity::LifeObjectComponent>(cubeEntity, "cube");
                RegistryBuffer.AddComponent<entity::Transform3DComponent>(
                        cubeEntity,
                        Camera.GetProjectionMatrix() * Camera.GetViewMatrix(),
                        glm::vec3{0.0f, 20.0f, 2},
                        glm::vec3{0.5f, 0.5f,0.5f},
                        glm::vec3{0.0f, 1.0f, 0.0f},
                        0
                );
                RegistryBuffer.AddComponent<entity::MeshRenderComponent>(
                        cubeEntity,
                        assets::ShaderPipelineType::Default,
                        new MeowEngine::StaticMeshInstance{
                                assets::StaticMeshType::Cube,
                                assets::TextureType::Pattern
                        }
                );
                RegistryBuffer.AddComponent<entity::ColliderComponent>(
                        cubeEntity,
                        entity::ColliderType::BOX,
                        new entity::BoxColliderData()
                );
                RegistryBuffer.AddComponent<entity::RigidbodyComponent>(
                        cubeEntity
                );
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
            Camera.Configure(CameraController.GetPosition(), CameraController.GetUp(), CameraController.GetDirection());

            const glm::mat4 cameraMatrix {Camera.GetProjectionMatrix() * Camera.GetViewMatrix()};

            auto view = RegistryBuffer.GetCurrent().view<entity::Transform3DComponent>();
            for(auto entity: view) {
                auto& transform = view.get<entity::Transform3DComponent>(entity);
                transform.Update(deltaTime);
                transform.CalculateTransformMatrix(cameraMatrix);
            }

        }

        void RenderGameView(MeowEngine::RenderSystem& renderer) {
            renderer.RenderGameView(&Camera, RegistryBuffer.GetCurrent());
        }

        void RenderUserInterface(MeowEngine::RenderSystem& renderer, unsigned int frameBufferId, const double fps) {
            renderer.RenderUserInterface(RegistryBuffer.GetCurrent(), RegistryBuffer.GetPropertyChangeQueue() , frameBufferId, fps);
        }

        void ApplyUpdateChanges() {

        }

        void ApplyPhysicSystemChanges() {
//            // Apply update physics transform to entities
//            auto view = RegistryBuffer.view<entity::Transform3DComponent, entity::RigidbodyComponent>();
//            for(auto entity: view)
//            {
//                auto& transform = view.get<entity::Transform3DComponent>(entity);
//                auto& rigidbody = view.get<entity::RigidbodyComponent>(entity);
//
//                // update transform using rigidbody transform;
//                rigidbody.UpdateTransform(transform);
//            }
        }

    };

    SceneSingleThread::SceneSingleThread(const MeowEngine::WindowSize& frameSize) : InternalPointer(MeowEngine::make_internal_ptr<Internal>(frameSize)) {}

    void SceneSingleThread::OnWindowResized(const MeowEngine::WindowSize &size) {
        InternalPointer->OnWindowResized(size);
    }

    void SceneSingleThread::LoadOnRenderSystem(std::shared_ptr<MeowEngine::AssetManager> assetManager) {
        InternalPointer->Load(assetManager);
    }
    void SceneSingleThread::CreateSceneOnMainSystem(MeowEngine::simulator::PhysicsSystem* inPhysics) {
        InternalPointer->Create(inPhysics);
    }

    void SceneSingleThread::Input(const float &deltaTime, const MeowEngine::input::InputManager& inputManager) {
        InternalPointer->Input(deltaTime, inputManager);
    }

    void SceneSingleThread::Update(const float &deltaTime) {
        InternalPointer->Update(deltaTime);
    }

    void SceneSingleThread::RenderGameView(MeowEngine::RenderSystem &renderer) {
        InternalPointer->RenderGameView(renderer);
    }

    void SceneSingleThread::RenderUserInterface(MeowEngine::RenderSystem &renderer, unsigned int frameBufferId, const double fps) {
        InternalPointer->RenderUserInterface(renderer, frameBufferId, fps);
    }

    void SceneSingleThread::ApplyUpdateChanges() {
        InternalPointer->ApplyUpdateChanges();
    }

    void SceneSingleThread::ApplyPhysicSystemChanges() {
        InternalPointer->ApplyPhysicSystemChanges();
    }


} // MeowEngine