## Physicat Engine

> [!CAUTION]
> ### Build Status (non-compilable)
> Implementing multi-threading which involves `main, render and physics thread` to
> handle different game states (produced & consumed).
> This involves decoupling the root engine code base and since it's still ongoing,
> currently build is not possible.
> 
- Physicat is a personal exploration into the world of physics engine development
- Focuses on:
  - Multithreading for optimized performance.
  - Profiling Tools for debugging and performance tracking.
  - Editor Tools
  - Physics Simulations

The ultimate goal of Physicat is to delve deep into the 
complexities of physics simulations and engine design,
providing a unique opportunity to learn and experiment 
with modern technologies and architectural concepts.

<img src="screenshots/engine_v3.gif" width="700">

## Compatibility
- [x] Unix Console on Mac
- [x] Mac (x84) (Rosetta)
- [ ] Windows
- [x] Web

> [!NOTE] 
> ### Build Status (non-compilable)
> Currently PhysX is non-compilable for web. Web builds are on halt.
> Will be revisiting once multithreading is implemented.

## Folder Structure
<img src="screenshots/directory-diagram.png">

## Dependencies
- Emscripten
- OpenGL ES3
  - Mac: OpenGLES/ES2/gl.h
  - Emscripten: GLES2/gl2.h
- SDL/SDL2 (mac framework) or GLFW
- Imgui
- Tracy Profiler
- PhysX
- [More tools...](markdowns/dependencies.md)
