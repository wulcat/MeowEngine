#!/bin/bash

. installer/platforms/shared-scripts.sh

# Default value for platform
#platform=""

# Parse command-line arguments
while [[ $# -gt 0 ]]; do
  case $1 in
    -p|--platform)
      platform="$2"
      shift 2
      ;;
    *)
      echo "Invalid option: $1" >&2
      exit 1
      ;;
  esac
done

# if platform = "" throw error

verify_build_folder_exists "$platform"
clean_ninja_folder "$platform"
clean_build_folder "$platform"

# Build using CMake
#cmake -B build -S .
#make -C build

# Build using Ninja
# shellcheck disable=SC2164
pushd installer/platforms/"$platform"/build
  # Request that CMake configures itself based on what it finds in the parent folder
  echo "Configuring CMake with Ninja ..."
  cmake -B build -G Ninja -D CMAKE_BUILD_TYPE=Debug ..

  # Start the build process
  echo "Building the project using Ninja ..."
  ninja

# shellcheck disable=SC2164
popd

# Run
#./out/"$platform"