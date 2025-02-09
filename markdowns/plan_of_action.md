## Plan of action

## Meow Engine v0.4
- [x] Add physics library (jolt / physx / box2d / bullet3) -> physx
- [x] Add FPS Counter
- [x] Implementing PhysX (left handed or right handed)
- [x] Multithreading for rendering and update loop
- [x] Thread for PhysicsSystem
- [x] Adding reflections for dynamic UI
- [x] Adding property changes to track UI updates
- [x] Creating objects in runtime (on start/end of main thread)
- [x] Bringing back VSync & Delta Time Calculations
- [x] Managing lifecycle of object and components
- [ ] Creating library for performing maths functions
- [ ] Reworking camera system and merging rotations with other objects in game using new custom math library
- [ ] Implement Tracy throughout engine especially for tracking memory allocations
- [ ] Basic simulation tests
- [x] Able to see physics colliders (or at least primitives)
- [ ] Break....... Rejuvenating

## Meow Engine v0.5
- [x] Compilation of physx for web
- [x] Single thread app builds for web
- [ ] Research on making dev/debug builds for in-depth testing
- [ ] Updating bash files for supporting physx on mac
- [ ] Creating vehicle using PhysX
- [ ] Position, Scale, Rotation handles after selecting objects
- [ ] Add Unit Testing
- [ ] Creating wrapper for switching between different physics engine
- [ ] If simple, add basic light and sky box
- [ ] Improving current shader pipeline to handle shaders dynamically
- [ ] Create Unique ID generator for items
- [ ] Documenting done items and different approaches and used libraries
- [ ] Researching on auto-build deployment and build status
- [ ] On repo update - auto build emscripten for web and upload
- [ ] Creating support for multi-scene compilation
- [ ] File Manager (Reading from file paths and avoiding enums)
- [ ] Ability to read/write on file for saving import references (instead of hard-coded enums)
- [ ] Create build modes for debug and release
- [ ] Find a way to isolate and build code while developing simulations
- [ ] HTML navigation multi scenes on demand (async download and load scenes)
- [ ] Support for window
- [ ] Create a template for dump-testing shaders
- [ ] Dynamic Window Layout & save'n'load
- [ ] Simple tool for importing files like textures/meshes
- [ ] Setting up Vulkan
- [ ] Creating 3D spatial hashing (showcase & in-built)

# Previous Versions
## Meow Engine v0.1
- [x] Setting up project for cross-platform build support
- [x] Import mesh/textures
- [x] Rendering in a scene
- [x] Keyboard & Mouse inputs
- [x] Window resize

## Meow Engine v0.2
- [x] Support for mac
- [x] Importing c# written fluid-simulation in this engine
- [x] Migrating 2D spatial hashing
- [x] Shader pipeline

## Meow Engine v0.3
- [x] Find a way to debug coding after built application
- [x] Add Proper logging & profiler system
- [x] Create Basic UI elements like a button and text
- [x] Documentation & UML Diagrams
- [x] Auto UML Creator tool
- [x] Implement ECS