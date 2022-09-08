#!/bin/bash

# Include the shared script from parent
. ../shared-scripts.sh

# Ask homebrew to install dependencies
fetch_brew_dependency "wget"
fetch_brew_dependency "cmake"
fetch_brew_dependency "ninja"

# Download SDL lib
fetch_third_party_lib_sdl
fetch_framework_sdl2
fetch_third_party_lib_glm
fetch_third_party_lib_tiny_obj_loader