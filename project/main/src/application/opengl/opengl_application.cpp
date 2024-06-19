//
// Created by Akira Lynn on 06/07/22.
//

#include "opengl_application.hpp"
#include "opengl_pipeline.hpp"
#include "opengl_mesh.hpp"

#include "../../core/wrappers/graphics_wrapper.hpp"
#include "../../core/logger/log.hpp"
#include "sdl_wrapper.hpp"


#include "../../core/assets/assets.hpp"
#include "../../core/cameras/perspective_camera.hpp"

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

    physicat::PerspectiveCamera CreateCamera() {
        std::pair<uint32_t, uint32_t> displaySize{physicat::sdl::GetDisplaySize()};

        return physicat::PerspectiveCamera{static_cast<float>(displaySize.first), static_cast<float>(displaySize.second)};
    }

    glm::mat4 CreateMeshTransform() {
        glm::mat4 identityMatrix{1.0f};

        glm::vec3 position{0.0f, 0.0f, 0.0f};
        glm::vec3 rotationAxis{0.0f, 1.0f, 0.0f};
        glm::vec3 scale{1.0f, 1.0f, 1.0f};

        float rotationDegrees{45.0f};

        return  glm::translate(identityMatrix, position) *
                glm::rotate(identityMatrix, glm::radians(rotationDegrees), rotationAxis) *
                glm::scale(identityMatrix, scale);
    }

} // namespace

struct OpenGLApplication::Internal {
    SDL_Window* Window;
    SDL_GLContext Context;

    const physicat::PerspectiveCamera Camera;
    const physicat::OpenGLPipeline DefaultPipeline;
    const physicat::OpenGLMesh Mesh;
    const glm::mat4 MeshTransform;

    Internal() : Window(physicat::sdl::CreateWindow(SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI)) ,
                 Context(CreateContext(Window)),
                 Camera(CreateCamera()),
                 DefaultPipeline(physicat::OpenGLPipeline("default")),
                 Mesh(physicat::OpenGLMesh(physicat::assets::LoadObjFile("assets/models/crate.obj"))),
                 MeshTransform(CreateMeshTransform())
    {
        //physicat::Log("CRATE!", "Crate has " );
    }

    ~Internal() {
        SDL_GL_DeleteContext(Context);
        SDL_DestroyWindow(Window);
    }

    void Render() const {
        SDL_GL_MakeCurrent(Window, Context);

        glClearColor(0.f, 0.f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const glm::mat4 mvp {
            Camera.GetProjectionMatrix() *
            Camera.GetViewMatrix() *
            MeshTransform
        };

        DefaultPipeline.Render(Mesh, mvp);

        SDL_GL_SwapWindow(Window);
    }
};

OpenGLApplication::OpenGLApplication() :
        InternalPointer(physicat::make_internal_ptr<Internal>())
{}

void OpenGLApplication::Render() {
    InternalPointer->Render();
}





















