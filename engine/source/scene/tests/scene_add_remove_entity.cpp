//
// Created by Akira Mujawar on 11/02/25.
//

#include "scene_add_remove_entity.hpp"

namespace MeowEngine {
    void SceneAddRemoveEntity::Update(const float &deltaTime) {
        SceneMultiThread::Update(deltaTime);

//        test2 += deltaTime;
//        test += deltaTime;
//
//        if(test2 > 1) {
//            test2 =0;
//            const auto cubeEntity = RegistryBuffer.AddEntity();
//            MeowEngine::Log("Added", static_cast<int>(cubeEntity));
//            RegistryBuffer.AddComponent<entity::LifeObjectComponent>(cubeEntity, "cube");
//            RegistryBuffer.AddComponent<entity::Transform3DComponent>(
//                    cubeEntity,
//                    Camera.GetProjectionMatrix() * Camera.GetViewMatrix(),
//                    glm::vec3{0.0f, 20.0f, 2},
//                    glm::vec3{0.5f, 0.5f,0.5f},
//                    glm::vec3{0.0f, 1.0f, 0.0f},
//                    0
//            );
//            RegistryBuffer.AddComponent<entity::MeshRenderComponent>(
//                    cubeEntity,
//                    assets::ShaderPipelineType::Default,
//                    new MeowEngine::StaticMeshInstance{
//                            assets::StaticMeshType::Cube,
//                            assets::TextureType::Pattern
//                    }
//            );
//            RegistryBuffer.AddComponent<entity::ColliderComponent>(
//                    cubeEntity,
//                    entity::ColliderType::BOX,
//                    new entity::BoxColliderData()
//            );
//            RegistryBuffer.AddComponent<entity::RigidbodyComponent>(
//                    cubeEntity
//            );
//        }
//        if(test > 2.01f) {
//            test = 0;
//            // remove entity
//
//            // add entity
//            for(auto entity: view) {
//
//                MeowEngine::Log("Remove", static_cast<int>(entity));
//                RegistryBuffer.RemoveEntity(entity);
//
//
//
////                MeowEngine::Log("Added Components", static_cast<int>(cubeEntity));
//                break;
//            }
//        }
    }

} // MeowEngine