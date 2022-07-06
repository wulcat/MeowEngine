
## Plan of action
- [x] Setting up project for cross-platform build support
- [ ] Import mesh/textures and rendering in a scene
- [ ] Keyboard & Mouse inputs
- [ ] Window resize
- [ ] Importing c# written fluid-simulation in this engine
- [ ] Setting up Vulkan
- [ ] Shader pipeline
- [ ] Creating 3D spatial hashing (showcase & in-built)
- [ ] Creating support for multi-scene compilation
- [ ] HTML navigation multi scenes on demand (async download and load scenes)
- [ ] Support for windows & mac

## Compitability
- [x] Console
- [ ] Mac
- [ ] Windows
- [x] Web

## Tools
- Emscriptten
- Vulkan
- OpenGL ES3
- SDL/SDL2 (mac framework) or GLFW

## References
- [Setting up Emscripten](https://emscripten.org/docs/getting_started/Tutorial.html#tutorial)
- [Building for cross platform](https://marcelbraghetto.github.io/a-simple-triangle/2019/03/02/part-01/)
- [Main Update in Emscripten](https://emscripten.org/docs/porting/emscripten-runtime-environment.html)
- [Pimpl Pattern](https://oliora.github.io/2015/12/29/pimpl-and-rule-of-zero.html)

## Setup
- Run project/console/setup.sh to install packages
- Run project/console/build.sh to build the project (auto runs the executable)
- Run project/emscripten/build.sh to build the project for web

## Issue Links to Query
#### If emcc on terminal won't work?
  Activate PATH and other environment variables in the current terminal `` $source ./emsdk_env.sh ``
  
## Shortcuts to note
- https://stackoverflow.com/questions/6780895/visual-studio-parameter-listing-intellisense

## Mini - TODO
- Find a way to shutdown the http.server python after running web build
- Explore what's happening in internal_ptr script with the templates and internal_ptr thing
- Empty namespaces name declaration & using namespaces vs include headers