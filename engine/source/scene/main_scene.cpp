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

using physicat::MainScene;
//using physicat::assets::ShaderPipelineType;
//using physicat::assets::StaticMeshType;
//using physicat::assets::TextureType;

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
    std::queue<physicat::ReflectionPropertyChange> UiInputPropertyChangesQueue;

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

    void Create(std::shared_ptr<physicat::simulator::Physics> inPhysics) {
//        inPhysics.Create();


        const auto meshEntity = Registry.create();
        Registry.emplace<entity::LifeObjectComponent>(meshEntity, "torus");
        Registry.emplace<entity::Transform3DComponent>(
            meshEntity,
            glm::vec3{0, 0, 0},
            glm::vec3{1.0, 1.0f, 1.0f},
            glm::vec3{0.0f, 1.0f, 0.0f},
            0.0f
        );
        Registry.emplace<entity::MeshRenderComponent>(
            meshEntity,
            assets::ShaderPipelineType::Default,
            new physicat::StaticMeshInstance{
                    assets::StaticMeshType::Torus,
                    assets::TextureType::Pattern
            }
        );

        const auto cubeEntity = Registry.create();
        Registry.emplace<entity::LifeObjectComponent>(cubeEntity, "cube");
        Registry.emplace<entity::Transform3DComponent>(
                cubeEntity,
                glm::vec3{0.0f, 0.0f, 2},
                glm::vec3{0.5f, 0.5f,0.5f},
                glm::vec3{0.0f, 1.0f, 0.0f},
                0
        );
        Registry.emplace<entity::MeshRenderComponent>(
                cubeEntity,
                assets::ShaderPipelineType::Default,
                new physicat::StaticMeshInstance{
                        assets::StaticMeshType::Cube,
                        assets::TextureType::Pattern
                }
        );
        Registry.emplace<entity::ColliderComponent>(
            cubeEntity,
            entity::ColliderType::BOX,
            new entity::BoxColliderData(&Registry.get<entity::Transform3DComponent>(cubeEntity))
        );
        Registry.emplace<entity::RigidbodyComponent>(
            cubeEntity
        );

        const auto cubeEntity1 = Registry.create();
        Registry.emplace<entity::LifeObjectComponent>(cubeEntity1, "cube1");
        Registry.emplace<entity::Transform3DComponent>(
                cubeEntity1,
                glm::vec3{0.0f, 0.0f, 2},
                glm::vec3{0.5f, 0.5f,0.5f},
                glm::vec3{0.0f, 1.0f, 0.0f},
                0
        );
        Registry.emplace<entity::MeshRenderComponent>(
                cubeEntity1,
                assets::ShaderPipelineType::Default,
                new physicat::StaticMeshInstance{
                        assets::StaticMeshType::Cube,
                        assets::TextureType::Pattern
                }
        );
        Registry.emplace<entity::ColliderComponent>(
                cubeEntity1,
                entity::ColliderType::BOX,
                new entity::BoxColliderData(&Registry.get<entity::Transform3DComponent>(cubeEntity1))
        );
        Registry.emplace<entity::RigidbodyComponent>(
                cubeEntity1
        );

        // setup object
        // later query for all rigidbody, get the physx, get the collider and construct for physics

        const auto gridEntity = Registry.create();
        Registry.emplace<entity::LifeObjectComponent>(gridEntity, "grid");
        Registry.emplace<entity::Transform3DComponent>(
                gridEntity,
                glm::vec3{0, 0, 0},
                glm::vec3{1.0, 1.0f, 1.0f},
                glm::vec3{0.0f, 1.0f, 0.0f},
                0.0f
        );
        Registry.emplace<entity::RenderComponentBase>(
            gridEntity,
            assets::ShaderPipelineType::Grid
        );

//        // TODO: can we have same class name with different namespaces?
//        // Creating physics for all bodies in game
//        for(auto &&[entity, transform, collider, rigidbody]
//        : Registry.view<entity::Transform3DComponent, entity::ColliderComponent, entity::RigidbodyComponent>().each()) {
//            inPhysics.AddRigidbody(transform, collider, rigidbody);
//        }

        physicat::Log("Creating", "Created");
    }

    void Input(const float& delta, const physicat::input::InputManager& inputManager) {
        if(!inputManager.isActive) {
            return;
        }

        if(inputManager.mouseState & SDL_BUTTON_LMASK) {
            CameraController.LookAround(inputManager.mouseDeltaX, inputManager.mouseDeltaY);
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

    void FixedUpdate(const float& inFixedDeltaTime, physicat::simulator::Physics& inPhysics) {
        inPhysics.Update(inFixedDeltaTime);

        // Apply update physics transform to entities
        auto view = Registry.view<entity::Transform3DComponent, entity::RigidbodyComponent>();
        for(auto entity: view)
        {
            auto& transform = view.get<entity::Transform3DComponent>(entity);
            auto& rigidbody = view.get<entity::RigidbodyComponent>(entity);

            // update transform using rigidbody transform;
            rigidbody.UpdateTransform(transform);
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

        auto view = Registry.view<entity::Transform3DComponent>();

        for(auto entity: view)
        {
            auto& transform = view.get<entity::Transform3DComponent>(entity);
            transform.Update(deltaTime, cameraMatrix);
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

    void SyncThreadData() {
        while(!UiInputPropertyChangesQueue.empty()) {
            ReflectionPropertyChange& change = UiInputPropertyChangesQueue.front();

            physicat::Reflection.ApplyPropertyChange(change, RegistryBuffer.GetCurrent());
            physicat::Reflection.ApplyPropertyChange(change, RegistryBuffer.GetStaging());
            physicat::Reflection.ApplyPropertyChange(change, RegistryBuffer.GetFinal());

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
void MainScene::Create(std::shared_ptr<physicat::simulator::Physics> inPhysics) {
    InternalPointer->Create(inPhysics);
}

void MainScene::Input(const float &deltaTime, const physicat::input::InputManager& inputManager) {
    InternalPointer->Input(deltaTime, inputManager);
}

void MainScene::FixedUpdate(const float& inFixedDeltaTime, physicat::simulator::Physics &inPhysics) {
    InternalPointer->FixedUpdate(inFixedDeltaTime, inPhysics);
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

void MainScene::SyncThreadData() {
    InternalPointer->SyncThreadData();
}












