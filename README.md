# Physicat 3D Simulation Engine

> [!CAUTION]
> #### Build Status (non-compilable)
> Implementing multi-threading which involves `main, render and physics thread` to
> handle different game states (produced & consumed).
> This involves decoupling the root engine code base and since it's still ongoing,
> currently build is not possible.

- Physicat is a personal exploration into the world of physics engine development
- Focuses on:
  - Multithreading for optimized performance.
  - Profiling tools for debugging and performance tracking.
  - Editor tools
  - Physics simulations
  - [Next plan of action...](markdowns/plan_of_action.md)

The ultimate goal of Physicat is to delve deep into the 
complexities of physics simulations and engine design,
providing a unique opportunity to learn and experiment 
with modern technologies and architectural concepts.

<img src="screenshots/engine_v3.gif" width="700">

## Compatibility 
- [x] Unix Console on Mac [(download)]()
- [x] Mac (x84) (Rosetta) [(download)]()
- [ ] Windows
- [x] Web [(demo)]()

> [!NOTE]
> Currently `PhysX` is non-compilable for web. `Web builds` are on halt.
> Will be revisiting once multithreading is implemented.

## Building & docs 
- [Compile Physicat Engine](markdowns/how_to_build.md)
- [Build documentation](markdowns/how_to_build.md)
- [See Versions](markdowns/versions.md)

## Dependencies [(full list)](markdowns/dependencies.md)
- Emscripten
- OpenGL ES3
  - Mac: OpenGLES/ES2/gl.h
  - Emscripten: GLES2/gl2.h
- SDL/SDL2 (mac framework) or GLFW
- Imgui
- Tracy Profiler
- PhysX
- DOxygen

## References [(learning resources)](markdowns/references.md)
While it may not cover everything, this list highlights the most valuable 
resources I've gathered during development.