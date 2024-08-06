#!/bin/bash

# Include the shared script from parent
. installer/platforms/shared-scripts.sh

# Ask homebrew to install dependencies
fetch_brew_dependency "wget"
fetch_brew_dependency "cmake"
fetch_brew_dependency "ninja"
fetch_brew_dependency "doxygen"
fetch_brew_dependency "graphviz"

# Download SDL lib
fetch_third_party_lib_sdl
fetch_third_party_lib_sdl_image

fetch_third_party_lib_glm
fetch_third_party_lib_tiny_obj_loader
fetch_third_party_lib_imgui
fetch_third_party_lib_tracy
fetch_third_party_lib_entt
fetch_third_party_lib_physx

fetch_framework_sdl2
fetch_framework_sdl2_image


