//
// Created by Akira Lynn on 06/07/22.
//

#include "opengl_application.hpp"
#include "opengl_pipeline.hpp"

#include "../../core/wrappers/graphics_wrapper.hpp"
#include "../../core/logger/log.hpp"
#include "../../core/wrappers/sdl_wrapper.hpp"


#include "../../core/assets/assets.hpp"

#include <string>

using physicat::OpenGLApplication;

namespace {
    SDL_GLContext CreateContext(SDL_Window* window) {
        static const std::string logTag("physicat::OpenGLApplication::CreateContext");

        SDL_GLContext context {SDL_GL_CreateContext(window)};

        #ifdef WIN32
            glewInit();
        #endif

        int viewportWidth;
        int viewportHeight;
        SDL_GL_GetDrawableSize(window, &viewportWidth, &viewportHeight);

        physicat::Log(logTag, "Created OpenGL Context with viewport size: " + std::to_string(viewportWidth) + " , " +
                              std::to_string(viewportHeight));

        glClearDepthf(1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_CULL_FACE);
        glViewport(0, 0, viewportWidth, viewportHeight);

        return context;
    }
} // namespace

struct OpenGLApplication::Internal {
    SDL_Window* Window;
    SDL_GLContext Context;
    const physicat::OpenGLPipeline DefaultPipeline;
    physicat::Mesh Mesh;

    Internal() : Window(physicat::sdl::CreateWindow(SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI)) ,
                Context(CreateContext(Window)),
                DefaultPipeline(physicat::OpenGLPipeline("default")),
                Mesh(physicat::assets::LoadObjFile("assets/models/crate.obj"))
    {
        physicat::Log("CRATE!", "Crate has " + std::to_string(Mesh.GetVertices().size()) + " vertices and " + std::to_string(Mesh.GetIndices().size()) + " indices.");
    }

    ~Internal() {
        SDL_GL_DeleteContext(Context);
        SDL_DestroyWindow(Window);
    }

    void Render() const {
        SDL_GL_MakeCurrent(Window, Context);

        glClearColor(0.3f, 0.7f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        SDL_GL_SwapWindow(Window);
    }
};

OpenGLApplication::OpenGLApplication() :
        InternalPointer(physicat::make_internal_ptr<Internal>())
{}

void OpenGLApplication::Render() {
    InternalPointer->Render();
}





















