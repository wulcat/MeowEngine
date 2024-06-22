//
// Created by Akira Mujawar on 21/06/24.
//

#include "main_scene.hpp"

#include "perspective_camera.hpp"
#include "static_mesh_instance.hpp"
#include "log.hpp"

using physicat::MainScene;
using physicat::assets::ShaderPipelineType;
using physicat::assets::StaticMeshType;
using physicat::assets::TextureType;

namespace {
    physicat::PerspectiveCamera CreateCamera(const float& width, const float& height) {
        return physicat::PerspectiveCamera(width, height);
    }
}

struct MainScene::Internal {
    physicat::PerspectiveCamera Camera;
    std::vector<physicat::StaticMeshInstance> StaticMeshes;

    Internal(const float& screenWidth, const float& screenHeight)
        : Camera(::CreateCamera(screenWidth, screenHeight)) {}

    void Create(physicat::AssetManager& assetManager) {
        assetManager.LoadShaderPipelines({ShaderPipelineType::Default});
        assetManager.LoadStaticMeshes({StaticMeshType::Crate});
        assetManager.LoadTextures({TextureType::Crate});

        StaticMeshes.push_back(physicat::StaticMeshInstance {StaticMeshType::Crate, TextureType::Crate});
    }

    // We can perform -> culling, input detection
    void Update(const float& deltaTime) {
        const glm::mat4 cameraMatrix {Camera.GetProjectionMatrix() * Camera.GetViewMatrix()};

//        physicat::Log("MainScene", "StaticMeshes Count" + StaticMeshes.size());

        for(auto& staticMesh : StaticMeshes) {
            staticMesh.RotateBy(deltaTime * 45.0f);
            staticMesh.Update(cameraMatrix);

//            physicat::Log("MainScene", "mesh");
        }
    }

    void Render(physicat::Renderer& renderer) {
        renderer.Render(ShaderPipelineType::Default, StaticMeshes);
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












