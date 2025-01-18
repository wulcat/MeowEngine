# Physicat Engine

### A Journey into Physics Engine Development

- Physicat is a personal exploration into the world of physics engine development
- Focuses on building a robust core architecture with:
  - Multithreading for optimized performance.
  - Profiling Tools for debugging and performance tracking.
  - Editor Tools

The ultimate goal of Physicat is to delve deep into the 
complexities of physics simulations and engine design,
providing a unique opportunity to learn and experiment 
with modern technologies and architectural concepts.

> [!CAUTION] ## Build Status (non-compilable)
> Implementing multi-threading which involves `main, render and physics thread` to
> handle different game states (produced & consumed).
> This involves decoupling the root engine code base and since it's still ongoing,
> currently build is not possible.

<img src="screenshots/engine_v3.gif" width="700">

## Compatibility
- [x] Unix Console on Mac
- [x] Mac (x84) (Rosetta)
- [ ] Windows
- [x] Web

> [!NOTE] ## Build Status (non-compilable)
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

Here is a simple footnote[^1].

A footnote can also have multiple lines[^2].

[^1]: My reference.
[^2]: To add line breaks within a footnote, prefix new lines with 2 spaces.
This is a second line.