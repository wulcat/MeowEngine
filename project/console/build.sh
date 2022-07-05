#!/bin/bash

. ../shared-scripts.sh

verify_build_folder_exists
clean_build_folder
ls
# Build using CMake
#cmake -B build -S .
#make -C build

# Build using Ninja
# shellcheck disable=SC2164
pushd build
  # Request that CMake configures iteself based on what it finds in the parent folder
  echo "Configuring CMake with Ninja ..."
  cmake -B build -G Ninja ..

  # Start the build process
  echo "Building the project using Ninja ..."
  ninja
# shellcheck disable=SC2164
popd

# Run
./out/engine-console