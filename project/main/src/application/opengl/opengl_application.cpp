//
// Created by Akira Mujawar on 06/07/22.
//

#include "opengl_application.hpp"

#include "graphics_wrapper.hpp"
#include "log.hpp"
#include "sdl_wrapper.hpp"
#include "main_scene.hpp"
#include "opengl_asset_manager.hpp"
#include "opengl_renderer.hpp"


#include <string>

using physicat::OpenGLApplication;

namespace {
    void UpdateViewport(SDL_Window* window) {
        static const std::string logTag("physicat::OpenGLApplication::UpdateViewport");

        int viewportWidth;
        int viewportHeight;

        SDL_GL_GetDrawableSize(window, &viewportWidth, &viewportHeight);
        physicat::Log(logTag, "Created OpenGL context with viewport size: "+ std::to_string(viewportWidth) + " x " + std::to_string(viewportHeight));

        glViewport(0,0, viewportWidth,viewportHeight);
    }

    SDL_GLContext CreateContext(SDL_Window* window) {
        static const std::string logTag("physicat::OpenGLApplication::CreateContext");

        SDL_GLContext context {SDL_GL_CreateContext(window)};

        #ifdef WIN32
            glewInit();
        #endif

        //int viewportWidth;
     //   int viewportHeight;
       // SDL_GL_GetDrawableSize(window, &viewportWidth, &viewportHeight);

      //  physicat::Log(logTag, "Created OpenGL Context with viewport size: " + std::to_string(viewportWidth) + " , " +
       //                       std::to_string(viewportHeight));

        glClearDepthf(1.0f);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glEnable(GL_CULL_FACE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//        glEnable(GL_DEPTH_TEST);
//        glDepthMask(GL_TRUE);
//        glDepthFunc(GL_LEQUAL);
//        glDepthRange(0.0f, 1.0f);
//        glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
//        glEnable(GL_SAMPLE_ALPHA_TO_ONE);
//        glEnable(GL_BLEND);
//        glBlendEquation(GL_FUNC_ADD);
//        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
//        glEnable(GL_ALPHA_TEST);
//        glAlphaFunc(GL_GREATER, 0.1f);

//        glViewport(0, 0, viewportWidth, viewportHeight);
        ::UpdateViewport(window);

        return context;
    }

    std::shared_ptr<physicat::OpenGLAssetManager> CreateAssetManager() {
        return std::make_shared<physicat::OpenGLAssetManager>(physicat::OpenGLAssetManager());
    }

    physicat::OpenGLRenderer CreateRenderer(std::shared_ptr<physicat::OpenGLAssetManager> assetManager) {
        return physicat::OpenGLRenderer(assetManager);
    }

    std::unique_ptr<physicat::Scene> CreateMainScene(SDL_Window* window, physicat::OpenGLAssetManager& assetManager) {
        std::unique_ptr<physicat::MainScene> mainScene {
            std::make_unique<physicat::MainScene>(
                physicat::sdl::GetWindowSize(window)
            )
        };

        mainScene->Create(assetManager);

        return mainScene;
    }

} // namespace

struct OpenGLApplication::Internal {
    SDL_Window* Window;
    SDL_GLContext Context;

    const std::shared_ptr<physicat::OpenGLAssetManager> AssetManager;
    physicat::OpenGLRenderer Renderer;
    std::unique_ptr<physicat::Scene> Scene;

    Internal() : Window(physicat::sdl::CreateWindow(SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI)) ,
                 Context(CreateContext(Window)),
                 AssetManager(::CreateAssetManager()),
                 Renderer(::CreateRenderer(AssetManager))
    {
        //physicat::Log("CRATE!", "Crate has " );
    }

    ~Internal() {
        SDL_GL_DeleteContext(Context);
        SDL_DestroyWindow(Window);
    }

    void OnWindowResized() {
        GetScene().OnWindowResized(physicat::sdl::GetWindowSize(Window));
        ::UpdateViewport(Window);
    }

    physicat::Scene& GetScene() {
        if(!Scene) {
            Scene = ::CreateMainScene(Window, *AssetManager);
        }

        return *Scene;
    }

    void Update(const float& deltaTime) {
        GetScene().Update(deltaTime);
    }

    void Render() {
        SDL_GL_MakeCurrent(Window, Context);

        glClearColor(10 / 255.0f, 10 / 255.0f, 10 / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GetScene().Render(Renderer);

        SDL_GL_SwapWindow(Window);
    }
};

OpenGLApplication::OpenGLApplication() :
        InternalPointer(physicat::make_internal_ptr<Internal>())
{}

void OpenGLApplication::OnWindowResized() {
    InternalPointer->OnWindowResized();
}

void OpenGLApplication::Update(const float &deltaTime) {
    InternalPointer->Update(deltaTime);
}

void OpenGLApplication::Render() {
    InternalPointer->Render();
}





















