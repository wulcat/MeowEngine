//
// Created by Akira Mujawar on 21/01/25.
//

#ifndef MEOWENGINE_OPENGL_APP_MULTI_THREAD_HPP
#define MEOWENGINE_OPENGL_APP_MULTI_THREAD_HPP


class opengl_app_multi_thread {

};


#endif //MEOWENGINE_OPENGL_APP_MULTI_THREAD_HPP

// physics system -> Physics
// render system -> Renderer

// PhysicsThread
// RenderThread

// Application
    // SingleThreadApplication -- selects the physics system
        // OpenGLApplicationSingleT
        // VulkanApplicationST
        // physics
            // PhysXPhysicsSingleThread
            // CustomPhysicsSingleT
    // MultiThreadApplication -- selects the physics thread which has physics system
        // opengl
            // OpenGLApplicationMultiT -- selects the correct render thread
            // OpenGLRenderMultiThread
        // vulkan
            // VulkanThreadApplication
            // VulkanRenderThread
        // physics
            // PhysXPhysicsThread
            // CustomPhysicsThread

// RenderSystem
    // OpenGL
    // Vulkan
// PhysicsSystem
    // PhysX
    // Custom