//
// Created by Akira Mujawar on 21/06/24.
//

#include "main_scene.hpp"

#include "camera_controller.hpp"
#include "perspective_camera.hpp"
#include "static_mesh_instance.hpp"
#include "log.hpp"
#include "opengl_line_pipeline.hpp"

#include "mesh_render_component.hpp"
#include "line_render_component.hpp"
#include "transform2d_component.hpp"
#include "transform3d_component.hpp"

#include "life_object_component.hpp"
#include "transform3d_component.hpp"
#include "box_collider_data.hpp"
#include "collider_component.hpp"

#include "rigidbody_component.hpp"
#include "entt_buffer.hpp"
#include "entt_reflection_wrapper.hpp"

#include "physx_physics.hpp"

using physicat::MainScene;


using namespace physicat::assets;
using namespace physicat::entity;

namespace {
    physicat::PerspectiveCamera CreateCamera(const physicat::WindowSize& size) {
        return physicat::PerspectiveCamera(static_cast<float>(size.Width), static_cast<float>(size.Height));
    }
}

// this -> type -> value
// component -> property -> property

struct MainScene::Internal {
    physicat::PerspectiveCamera Camera;
    physicat::CameraController CameraController;

    EnttBuffer RegistryBuffer;
    std::queue<std::shared_ptr<physicat::ReflectionPropertyChange>> UiInputPropertyChangesQueue;
    moodycamel::ConcurrentQueue<std::shared_ptr<physicat::ReflectionPropertyChange>> PhysicsUiInputPropertyChangesQueue;


    float Time;

    // User Input Events
    const uint8_t* KeyboardState; // SDL owns the object & will manage the lifecycle. We just keep a pointer.

    Internal(const physicat::WindowSize& size)
        : Camera(::CreateCamera(size))
        , CameraController({glm::vec3(0.0f, 2.0f , -10.0f)})
        , KeyboardState(SDL_GetKeyboardState(nullptr))
        , Time(0)
        , RegistryBuffer()
    {}

    void OnWindowResized(const physicat::WindowSize& size) {
        Camera = ::CreateCamera(size);
    }

    void Load(std::shared_ptr<physicat::AssetManager> assetManager) {
        assetManager->LoadShaderPipelines({
                                                  assets::ShaderPipelineType::Grid,
                                                  assets::ShaderPipelineType::Default,
                                                  assets::ShaderPipelineType::Line
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
    }

    void CreatePhysics(physicat::simulator::Physics* inPhysics) {
        RegistryBuffer.CreateInStaging();
        RegistryBuffer.AddInStaging(inPhysics);
    }

    void Create() {
        auto entity = RegistryBuffer.Create();
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
                new physicat::StaticMeshInstance{
                        assets::StaticMeshType::Torus,
                        assets::TextureType::Pattern
                }
        );

        const auto cubeEntity = RegistryBuffer.Create();
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
                new physicat::StaticMeshInstance{
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

        const auto cubeEntity1 = RegistryBuffer.Create();
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
                new physicat::StaticMeshInstance{
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

        // setup object
        // later query for all rigidbody, get the physx, get the collider and construct for physics

        const auto gridEntity = RegistryBuffer.Create();
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

        physicat::Log("Creating", "Created");
    }

    void Input(const float& delta, const physicat::input::InputManager& inputManager) {
        if(!inputManager.isActive) {
            return;
        }

        if(inputManager.mouseState & SDL_BUTTON_LMASK) {
            CameraController.LookAround(inputManager.mouseDeltaX, inputManager.mouseDeltaY);
        }

        if(inputManager.isMouseDown && (inputManager.mouseState & SDL_BUTTON_RMASK)) {
            const auto cubeEntity = RegistryBuffer.Create();
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
                    new physicat::StaticMeshInstance{
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

    void FixedUpdate(const float& inFixedDeltaTime) {
//        inPhysics.Update(inFixedDeltaTime);

//        // Apply update physics transform to entities
        auto view = RegistryBuffer.GetStaging().view<entity::Transform3DComponent, entity::RigidbodyComponent>();
        for(auto entity: view)
        {
            auto& transform = view.get<entity::Transform3DComponent>(entity);
            auto& rigidbody = view.get<entity::RigidbodyComponent>(entity);

            rigidbody.UpdateTransform(transform);
        }

        std::shared_ptr<physicat::ReflectionPropertyChange> change;
        while(PhysicsUiInputPropertyChangesQueue.try_dequeue(change)) {
            if(view.contains(static_cast<entt::entity>(change->EntityId))) {
                physicat::Reflection.ApplyPropertyChange(*change, RegistryBuffer.GetStaging());
                auto [transform, rigidbody] = view.get<entity::Transform3DComponent, entity::RigidbodyComponent>(static_cast<entt::entity>(change->EntityId));
                rigidbody.OverrideTransform(transform);
            }
        }
    }

    // We can perform -> culling, input detection
    void Update(const float& deltaTime) {
        Time = deltaTime;
        
        Camera.Configure(CameraController.GetPosition(), CameraController.GetUp(), CameraController.GetDirection());

        const glm::mat4 cameraMatrix {Camera.GetProjectionMatrix() * Camera.GetViewMatrix()};

//        for(auto& lifeObject : LifeObjects) {
//            lifeObject.TransformComponent->Update(cameraMatrix);
//        }

//        physicat::Log("Camera", std::to_string(Camera.GetPosition().z));

        auto view = RegistryBuffer.GetCurrent().view<entity::Transform3DComponent>();
        for(auto entity: view) {
            auto& transform = view.get<entity::Transform3DComponent>(entity);
            transform.Update(deltaTime);
            transform.CalculateTransformMatrix(cameraMatrix);
        }

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
//        for(auto &&[entity,renderComponent, transform]: registry.view<entity::MeshRenderComponent, entity::Transform3DComponent>().each())
//        {
//            AssetManager->GetShaderPipeline<OpenGLMeshPipeline>(ShaderPipelineType::Default)->Render(
//                    *AssetManager,
//                    &renderComponent,
//                    &transform
//            );
//        }
//
//        for(auto &&[entity,renderComponent, transform]: registry.view<entity::RenderComponentBase, entity::Transform3DComponent>().each())
//        {
//            AssetManager->GetShaderPipeline<OpenGLGridPipeline>(ShaderPipelineType::Grid)->Render(
//                    *AssetManager,
//                    &renderComponent,
//                    &transform,
//                    cameraObject
//            );
//        }
    }

    void Render(physicat::Renderer& renderer) {
        // This is important for now - we can come to this later for optimization
        // Current goal is to have full control on render as individual objects
        // as we will have elements like UI, Static Meshes, Post Processing, Camera Culling, Editor Tools
//        for(auto& lifeObject : LifeObjects) {
//            renderer.Render(&Camera, &lifeObject);
//        }
        renderer.Render(&Camera, RegistryBuffer.GetFinal());
    }

    void RenderUI(physicat::Renderer& renderer, unsigned int frameBufferId, const double fps) {
        renderer.RenderUI(RegistryBuffer.GetFinal(), UiInputPropertyChangesQueue , frameBufferId, fps);
    }

    void SwapBuffer() {
        RegistryBuffer.Swap();
    }

    void CalculateDeltaData() {

        auto currentView = RegistryBuffer.GetCurrent().view<physicat::entity::Transform3DComponent, physicat::entity::RigidbodyComponent>();
        auto stagingView = RegistryBuffer.GetStaging().view<physicat::entity::Transform3DComponent, physicat::entity::RigidbodyComponent>();
        auto finalView = RegistryBuffer.GetFinal().view<physicat::entity::Transform3DComponent, physicat::entity::RigidbodyComponent>();

        for(entt::entity entity : stagingView) {

            auto& staging = stagingView.get<physicat::entity::RigidbodyComponent>(entity);
            auto final = finalView.get<physicat::entity::Transform3DComponent>(entity);
            auto current = currentView.get<physicat::entity::Transform3DComponent>(entity);

            staging.CacheDelta(current.Position - final.Position);
        }
    }

    void SyncPhysicsThreadData() {
        auto currentView = RegistryBuffer.GetCurrent().view<physicat::entity::Transform3DComponent, physicat::entity::RigidbodyComponent>();
        auto stagingView = RegistryBuffer.GetStaging().view<physicat::entity::Transform3DComponent, physicat::entity::RigidbodyComponent>();
        auto finalView = RegistryBuffer.GetFinal().view<physicat::entity::Transform3DComponent, physicat::entity::RigidbodyComponent>();

        for(entt::entity entity : stagingView) {
            auto staging = stagingView.get<physicat::entity::Transform3DComponent>(entity);
            auto& rigidbody = stagingView.get<physicat::entity::RigidbodyComponent>(entity);
            auto& final = finalView.get<physicat::entity::Transform3DComponent>(entity);

            auto& current = currentView.get<physicat::entity::Transform3DComponent>(entity);

            rigidbody.AddDelta(current.Position - final.Position);
            current.Position = staging.Position;
        }
    }

    void SyncThreadData() {
        // Add a template method to apply changes
        // Each component will have a apply data method
        // We will have data and component methods
        // Data will always my permanant with no apply data method
        // Components will always have apply method
        // Sync Transform Component
        auto currentView = RegistryBuffer.GetCurrent().view<physicat::entity::Transform3DComponent>();
        auto finalView = RegistryBuffer.GetFinal().view<physicat::entity::Transform3DComponent>();

        for(entt::entity entity : currentView) {
            auto current = currentView.get<physicat::entity::Transform3DComponent>(entity);
            auto& final = finalView.get<physicat::entity::Transform3DComponent>(entity);

            final.Position = current.Position;
        }

        // create a template method
        // pass the component
        // every component needs to have a sync method

        // Apply UI Inputs
        // can go to our extended entt buffer class
        while(!UiInputPropertyChangesQueue.empty()) {
            std::shared_ptr<physicat::ReflectionPropertyChange> change = UiInputPropertyChangesQueue.front();

            physicat::Reflection.ApplyPropertyChange(*change.get(), RegistryBuffer.GetCurrent());
            physicat::Reflection.ApplyPropertyChange(*change.get(), RegistryBuffer.GetFinal());

            PhysicsUiInputPropertyChangesQueue.enqueue(change);
            UiInputPropertyChangesQueue.pop();
        }
    }
};

MainScene::MainScene(const physicat::WindowSize& size)
    : InternalPointer(physicat::make_internal_ptr<Internal>(size)){}

void MainScene::OnWindowResized(const physicat::WindowSize &size) {
    InternalPointer->OnWindowResized(size);
}

void MainScene::Load(std::shared_ptr<physicat::AssetManager> assetManager) {
    InternalPointer->Load(assetManager);
}
void MainScene::Create() {
    InternalPointer->Create();
}

void MainScene::CreatePhysics(physicat::simulator::Physics* inPhysics) {
    InternalPointer->CreatePhysics(inPhysics);
}

void MainScene::Input(const float &deltaTime, const physicat::input::InputManager& inputManager) {
    InternalPointer->Input(deltaTime, inputManager);
}

void MainScene::FixedUpdate(const float& inFixedDeltaTime) {
    InternalPointer->FixedUpdate(inFixedDeltaTime);
}

void MainScene::Update(const float &deltaTime) {
    InternalPointer->Update(deltaTime);
}

void MainScene::Render(physicat::Renderer &renderer) {
    InternalPointer->Render(renderer);
}

void MainScene::RenderUI(physicat::Renderer &renderer, unsigned int frameBufferId, const double fps) {
    InternalPointer->RenderUI(renderer, frameBufferId, fps);
}

const float& MainScene::GetDeltaTime() {
    return InternalPointer->Time ;
}

void MainScene::SwapBuffer() {
    InternalPointer->SwapBuffer();
}

void MainScene::CalculateDeltaData() {
    InternalPointer->CalculateDeltaData();
}
void MainScene::SyncPhysicsThreadData() {
    InternalPointer->SyncPhysicsThreadData();
}
void MainScene::SyncThreadData() {
    InternalPointer->SyncThreadData();
}












