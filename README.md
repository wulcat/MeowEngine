
## Compitability
- [x] Executable
- [ ] Mac
- [ ] Windows
- [x] Web

## Tools
- Emscriptten
- Vulkan
- OpenGL ES3
- SDL/SDL2 (mac framework) or GLFW

## Tutorials
- [Setting up Emscripten](https://emscripten.org/docs/getting_started/Tutorial.html#tutorial)
- [Building for cross platform](https://marcelbraghetto.github.io/a-simple-triangle/2019/03/02/part-01/)
- [Main Loop in Emscripten](https://emscripten.org/docs/porting/emscripten-runtime-environment.html)

## Setup
- Run project/console/setup.sh to install packages
- Run project/console/build.sh to build the project (auto runs the executable)
- Run project/emscripten/build.sh to build the project for web

## Issue Links to Query
#### If emcc on terminal won't work?
  Activate PATH and other environment variables in the current terminal `` $source ./emsdk_env.sh ``