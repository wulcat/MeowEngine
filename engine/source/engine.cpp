//
// Created by Akira Mujawar on 06/07/22.
//

#include "engine.hpp"

//#include "application.hpp"
//#include "opengl_application.hpp"
#include "application_test.hpp"

#include "log.hpp"
#include "sdl_wrapper.hpp"

#include <stdexcept>
#include <string>
#include "SDL_image.h"

using MeowEngine::Engine;

struct Engine::Internal {
    const std::string classLogTag;

    Internal() : classLogTag("MeowEngine::Engine::") {}

    void Run() {
        PT_PROFILE_SCOPE;
        static const std::string logTag{classLogTag + "run"};

        MeowEngine::Log(logTag, "Initializing Engine...");

//        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

//        MeowEngine::Log(logTag, "SDL2 Initialized");
//
//        if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
//            throw std::runtime_error(logTag + "Could not initialize SDL2_image");
//        }

//        MeowEngine::Log(logTag, "SDL2_image initialized (supports png)");

        ResolveApplication()->StartApplication();
    }

    // we are using unique_ptr smart pointer as a return value so it will self destruct upon leaving its containing scope
    std::unique_ptr<MeowEngine::ApplicationTest> ResolveApplication() {
        PT_PROFILE_SCOPE;
        static const std::string logTag{classLogTag + "resolving Application"};

        try {
            MeowEngine::Log(logTag, "Creating OpenGL Application...");
//            return std::make_unique<MeowEngine::OpenGLApplication>();
            return std::make_unique<MeowEngine::ApplicationTest>();
        }
        catch (const std::exception& error) {
            MeowEngine::Log(logTag, "OpenGL Application failed to initialized.", error);
        }

        throw std::runtime_error(logTag + " No application can run on current environment");
    }

    ~Internal() {
        SDL_Quit();
    }
};

Engine::Engine() : InternalPointer(MeowEngine::make_internal_ptr<Internal>()) {}

void Engine::Run() {
    InternalPointer->Run();
}

