#!/bin/bash

#Include the shared scripts
. ../shared-scripts.sh

#Ask homebrew to install dependencies
fetch_brew_dependency "wget"
fetch_brew_dependency "xcodegen"

# Download SDL lib
fetch_third_party_lib_sdl
fetch_framework_sdl2
fetch_third_party_lib_glm
fetch_third_party_lib_tiny_obj_loader

# check to see if we have an existing symlink to our shared main c++ source folder
if [ ! -d "Source" ]; then
  echo "Linking 'Source' path to '..main/src'"
  ln -s ../main/src Source
fi

# Create Xcode project
echo "Generating Xcode project"
xcodegen generate


