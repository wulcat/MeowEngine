//
// Created by Akira Mujawar on 21/06/24.
//

#ifndef MEOWENGINE_MAIN_SCENE_HPP
#define MEOWENGINE_MAIN_SCENE_HPP

#include "internal_ptr.hpp"
#include "scene.hpp"
#include "window_size.hpp"

namespace MeowEngine {
    struct MainScene : public MeowEngine::Scene {
        MainScene(const MeowEngine::WindowSize& frameSize);

        void OnWindowResized(const MeowEngine::WindowSize& size) override;

        virtual void LoadOnRenderThread(std::shared_ptr<MeowEngine::AssetManager> assetManager) override;
        void CreateSceneOnMainThread() override;
        void AddEntitiesOnPhysicsThread(MeowEngine::simulator::Physics* inPhysics) override;
        void Input(const float &deltaTime, const MeowEngine::input::InputManager& inputManager) override;

        void Update(const float& deltaTime) override;
        void RenderGameView(MeowEngine::Renderer& renderer) override;
        void RenderUserInterface(MeowEngine::Renderer& renderer, unsigned int frameBufferId, const double fps) override;
        void SwapMainAndRenderBufferOnMainThread() override;
//        void CalculateDeltaData() override;
        void SyncPhysicsBufferOnMainThread(bool inIsPhysicsThreadWorking) override;
        void SyncRenderBufferOnMainThread() override;
        void SyncPhysicsBufferOnPhysicsThread() override;

    private:
        struct Internal;
        MeowEngine::internal_ptr<Internal> InternalPointer;
    };
}


#endif //MEOWENGINE_MAIN_SCENE_HPP
