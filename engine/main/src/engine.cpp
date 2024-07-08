//
// Created by Akira Lynn on 06/07/22.
//

#include "engine.hpp"

#include "application/application.hpp"
#include "src/application/graphics/opengl/opengl_application.hpp"
#include "log.hpp"
#include "sdl_wrapper.hpp"
#include <stdexcept>
#include <string>
#include "SDL_image.h"

using physicat::Engine;

struct Engine::Internal {
    const std::string classLogTag;

    Internal() :
        classLogTag("physicat::Engine::") {
    }

    void Run() {
        static const std::string logTag{classLogTag + "run"};

        physicat::Log(logTag, "Initializing Engine...");

        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

        physicat::Log(logTag, "SDL2 Initialized");

        if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
            throw std::runtime_error(logTag + "Could not initialize SDL2_image");
        }

        physicat::Log(logTag, "SDL2_image initialized (supports png)");

        ResolveApplication()->StartApplication();
    }

    // we are using unique_ptr smart pointer as a return value so it will self destruct upon leaving its containing scope
    std::unique_ptr<physicat::Application> ResolveApplication() {
        static const std::string logTag{classLogTag + "resolving Application"};

        try {
            physicat::Log(logTag, "Creating OpenGL Application...");
            return std::make_unique<physicat::OpenGLApplication>();
        }
        catch (const std::exception& error) {
            physicat::Log(logTag, "OpenGL Application failed to initialized.", error);
        }

        throw std::runtime_error(logTag + " No application can run on current environment");
    }

    ~Internal() {
        SDL_Quit();
    }
};

Engine::Engine() : InternalPointer(physicat::make_internal_ptr<Internal>()) {}

void Engine::Run() {
    InternalPointer->Run();
}

