//
// Created by Akira Mujawar on 21/06/24.
//

#ifndef PHYSICAT_MAIN_SCENE_HPP
#define PHYSICAT_MAIN_SCENE_HPP

#include "internal_ptr.hpp"
#include "scene.hpp"
#include "window_size.hpp"

namespace MeowEngine {
    struct MainScene : public MeowEngine::Scene {
        MainScene(const MeowEngine::WindowSize& frameSize);

        void OnWindowResized(const MeowEngine::WindowSize& size) override;

        virtual void Load(std::shared_ptr<MeowEngine::AssetManager> assetManager) override;
        void Create() override;
        void CreatePhysics(MeowEngine::simulator::Physics* inPhysics) override;
        void Input(const float &deltaTime, const MeowEngine::input::InputManager& inputManager) override;
        void FixedUpdate(const float& inFixedDeltaTime) override;
        void Update(const float& deltaTime) override;
        void Render(MeowEngine::Renderer& renderer) override;
        void RenderUI(MeowEngine::Renderer& renderer, unsigned int frameBufferId, const double fps) override;
        void SwapBuffer() override;
        void CalculateDeltaData() override;
        void SyncPhysicsThreadData() override;
        void SyncThreadData() override;

        const float& GetDeltaTime() override;

    private:
        struct Internal;
        MeowEngine::internal_ptr<Internal> InternalPointer;
    };
}


#endif //PHYSICAT_MAIN_SCENE_HPP
