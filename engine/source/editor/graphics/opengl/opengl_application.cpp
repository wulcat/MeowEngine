//
// Created by Akira Mujawar on 06/07/22.
//

#include "opengl_application.hpp"

#include "graphics_wrapper.hpp"
#include "sdl_wrapper.hpp"
#include "log.hpp"

#include "opengl_renderer.hpp"
#include "imgui_renderer.hpp"

#include "opengl_framebuffer.hpp"
#include "opengl_asset_manager.hpp"
#include "input_manager.hpp"
#include "main_scene.hpp"
#include "physx_physics.hpp"
#include <fps_counter.hpp>
#include <string>

using physicat::OpenGLApplication;

namespace {
//    void UpdateViewport(SDL_Window* window) {
//        static const std::string logTag("physicat::OpenGLApplication::UpdateViewport");
//
//        int viewportWidth;
//        int viewportHeight;
//
//        SDL_GL_GetDrawableSize(window, &viewportWidth, &viewportHeight);
//        physicat::Log(logTag, "Created OpenGL context with viewport size: "+ std::to_string(viewportWidth) + " x " + std::to_string(viewportHeight));
//
//        glViewport(0,0, viewportWidth,viewportHeight);
//    }

    SDL_GLContext CreateContext(SDL_Window* window) {
        static const std::string logTag("physicat::OpenGLApplication::CreateContext");

        SDL_GLContext context {SDL_GL_CreateContext(window)};
        SDL_GL_SetSwapInterval(1);// wth si this lol? from 100-450 to 1700 fps?
        SDL_GL_MakeCurrent(window, context);
        #ifdef WIN32
            glewInit();
        #endif

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

//        ::UpdateViewport(window);
        physicat::Log(logTag, "Context Created");

        // Check WebGL version
        const char* version = (const char*)glGetString(GL_VERSION);
        physicat::Log("physicat::graphics::OpenGLFrameBuffer: WEBGL Version", version);

        return context;
    }

    physicat::graphics::OpenGLFrameBuffer CreateFrameBuffer() {
        // NOTE: we launch this needs to be init properly
        return physicat::graphics::OpenGLFrameBuffer(1000,500);
    }

    std::shared_ptr<physicat::OpenGLAssetManager> CreateAssetManager() {
        return std::make_shared<physicat::OpenGLAssetManager>(physicat::OpenGLAssetManager());
    }

    physicat::OpenGLRenderer CreateRenderer(std::shared_ptr<physicat::OpenGLAssetManager> assetManager) {
        return physicat::OpenGLRenderer(assetManager);
    }

    physicat::graphics::ImGuiRenderer CreateUI(SDL_Window* window, SDL_GLContext& context) {
        return physicat::graphics::ImGuiRenderer(window, context);
    }

    std::shared_ptr<physicat::simulator::Physics> CreatePhysics() {
        return std::make_shared<physicat::simulator::PhysXPhysics>();
    }

    std::unique_ptr<physicat::Scene> CreateMainScene(SDL_Window* window, physicat::OpenGLAssetManager& assetManager, physicat::simulator::Physics& inPhysics) {
        std::unique_ptr<physicat::MainScene> mainScene {
            std::make_unique<physicat::MainScene>(
                physicat::sdl::GetWindowSize(window)
            )
        };

//        mainScene->Create(assetManager, inPhysics);

        return mainScene;
    }

} // namespace

struct OpenGLApplication::Internal {
    SDL_Window* Window;
    SDL_GLContext Context;
    physicat::graphics::ImGuiRenderer UI;
    physicat::graphics::OpenGLFrameBuffer FrameBuffer;
    physicat::input::InputManager InputManager;

    const std::shared_ptr<physicat::OpenGLAssetManager> AssetManager;
    physicat::OpenGLRenderer Renderer;
    std::unique_ptr<physicat::Scene> Scene;
    std::shared_ptr<physicat::simulator::Physics> Physics;

    Internal() : Window(physicat::sdl::CreateWindow(SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI)) ,
                 Context(CreateContext(Window)),
                 AssetManager(::CreateAssetManager()),
                 Renderer(::CreateRenderer(AssetManager)),
                 UI(CreateUI(Window, Context)),
                 FrameBuffer(::CreateFrameBuffer()),
                 InputManager(),
                 Physics(::CreatePhysics())
    {}

    ~Internal() {
        SDL_GL_DeleteContext(Context);
        SDL_DestroyWindow(Window);
    }

//    void OnWindowResized() {
//        physicat::Log("opengl_application::", "OnWindowResized");
//        GetScene().OnWindowResized(physicat::sdl::GetWindowSize(Window));
//        ::UpdateViewport(Window);
//    }

    void OnViewportResize(const WindowSize& size) {
        GetScene().OnWindowResized(size);
        glViewport(0,0, size.Width,size.Height);
        FrameBuffer.RescaleFrameBuffer(size.Width, size.Height);
    }

    physicat::Scene& GetScene() {
        if(!Scene) {
            Scene = ::CreateMainScene(Window, *AssetManager, *Physics);
        }

        return *Scene;
    }

    bool Input(const float& deltaTime) {
        PT_PROFILE_SCOPE;
        SDL_Event event;

        // Each loop we will process any events that are waiting for us.
        while (SDL_PollEvent(&event))
        {
            UI.Input(event);

            switch (event.type)
            {
                case SDL_WINDOWEVENT:
//                    if(event.window.event == SDL_WINDOWEVENT_RESIZED) {
//                        OnWindowResized();
//                    }
                    break;

                    // If we get a quit signal, we will return that we don't want to keep looping.
                case SDL_QUIT:
                    UI.ClosePIDs();
                    return false;

                case SDL_KEYDOWN:
                    // If we get a key down event for the ESC key, we also don't want to keep looping.
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        UI.ClosePIDs();
                        return false;
                    }
                    break;

                case SDL_USEREVENT:
                    switch (event.user.code) {
                        case 2: {
                            const WindowSize size = *(WindowSize *) event.user.data1;
                            OnViewportResize(size);
                            break;
                        }
                        case 3: {
                            InputManager.isActive = *(bool *) event.user.data1;
                            break;
                        }
                    }
                default:
                    break;
            }
        }

        // Track keyboard and mouse clicks/hold/drag/position
        InputManager.ProcessInput();

        GetScene().Input(deltaTime, InputManager);

        return true;
    }

    void FixedUpdate(const float& inFixedDeltaTime) {
        PT_PROFILE_SCOPE;
        GetScene().FixedUpdate(inFixedDeltaTime, *Physics);
    }

    void Update(const float& deltaTime) {
        PT_PROFILE_SCOPE;
        GetScene().Update(deltaTime);
    }

    double targetFrameTime = 1.0 / 240.0;  // Targeting 60 FPS
    double frameStartTime = SDL_GetTicks();
    Uint64 frequency = SDL_GetPerformanceFrequency();
    Uint64 previousTime = SDL_GetPerformanceCounter();

    void Render() {
        PT_PROFILE_SCOPE;

        Uint64 currentTime = SDL_GetPerformanceCounter();
        {
            PT_PROFILE_SCOPE_N("setting current");
            // We let opengl know that any after this will be drawn into custom frame buffer
            {
                PT_PROFILE_SCOPE_N("framebuffer");
                FrameBuffer.Bind();

                {
                    PT_PROFILE_SCOPE_N("scene render");
                    glClearColor(50 / 255.0f, 50 / 255.0f, 50 / 255.0f, 1.0f);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
                    GetScene().Render(Renderer);
//
                    FrameBuffer.Unbind();
                }
            }

            glClearColor(50 / 255.0f, 50 / 255.0f, 50 / 255.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            {
                PT_PROFILE_SCOPE_N("UI render");
                UI.Render(GetScene(), FrameBuffer.GetFrameTexture(), FpsCounter.getSmoothFPS());
            }

            {
                PT_PROFILE_SCOPE_N("waiting");


                // Frame timing logic
                Uint64 frameEndTime = SDL_GetPerformanceCounter();
                double frameDuration = (double) (frameEndTime - currentTime) / frequency;

                while (frameDuration < targetFrameTime) {
                    frameEndTime = SDL_GetPerformanceCounter();
                    frameDuration = (double) (frameEndTime - currentTime) / frequency;
                }

                previousTime = frameEndTime;
            }
                {FpsCounter.frameEnd();
                    PT_PROFILE_SCOPE_N("swap");
                    SDL_GL_SwapWindow(Window);



//                // Manual Frame Synchronization --
//                // Insert a fence sync object at the end of the previous frame's commands
//                GLsync sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
//                // Wait until the fence is signaled, meaning the previous frame is done
//                GLenum result = glClientWaitSync(sync, GL_SYNC_FLUSH_COMMANDS_BIT, GLuint64(1e9)); // Timeout in nanoseconds
//
//                // Optionally handle the result to check if it timed out or was successful
//                if (result == GL_WAIT_FAILED) {
//                    // Handle the error
//                }
//
//                // Clean up the sync object after use
//                glDeleteSync(sync);
//                 -- end
            }
        }
    }
};

OpenGLApplication::OpenGLApplication() :
        InternalPointer(physicat::make_internal_ptr<Internal>())
{
    physicat::Log("physicat::Application::OpenGLApplication", "OpenGL application created");
}

//void OpenGLApplication::OnWindowResized() {
//    InternalPointer->OnWindowResized();
//}

bool OpenGLApplication::Input(const float& deltaTime) {
    return InternalPointer->Input(deltaTime);
}

void OpenGLApplication::FixedUpdate(const float& inFixedDeltaTime) {
    InternalPointer->FixedUpdate(inFixedDeltaTime);
}

void OpenGLApplication::Update(const float &deltaTime) {
    InternalPointer->Update(deltaTime);
}

void OpenGLApplication::Render() {
    InternalPointer->Render();
}





















