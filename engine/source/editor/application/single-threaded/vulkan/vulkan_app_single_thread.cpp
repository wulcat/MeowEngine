//
// Created by Akira Mujawar on 19/10/24.
//

#include "vulkan_app_single_thread.hpp"

// - create renderer (game & ui)
// - create physics simulator
// - create input manager
// - create scene

// - load assets
// - create scene data (uses loaded assets)
// - create physics data (uses scene data & loaded assets)

// -------------------- thread updates start

// loop physics thread
// - update
// - process request for creating life object

// loop render thread
// - process input requests from buffer
// - update game using final buffer
// - update ui using final buffer (queue for any ui inputs)
// - swap (internal render buffer)

// loop main thread
// - game update
// - input
//   - device input
//   - process input manager
//   - scene input
// - swap input buffer
// - sync thread data
//   - apply current buffer to final buffer
//   - apply ui changes from queue to current & final buffer (physics in tbd)
//   - add life objects as per requests - (can be from ui inputs / game update)
//   - queue request for physics to added life objects
// - swap game buffer

// -------------------- thread updates end

// - pause rendering
// - pause physics simulator updates
// - pause scene updates

// - destroy physics simulator
// - destroy scene data
// - destroy loaded assets


// ------------------
// main thread

// create scene data
// starts looping
// ends

// ------------------
// render thread

// load assets
// starts looping
// ends

// ------------------
// physics thread

// create physics data
// starts looping
// ends

