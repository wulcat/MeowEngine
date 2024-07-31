#!/bin/bash


# Starts throwing error
## Alias the 'pushd' command and have it send its output to the abyss ...
#pushd() {
#    command pushd "$@" > /dev/null
#}
#
## Alias the 'popd' command and have it send its output to the abyss ...
#popd() {
#    command popd "$@" > /dev/null
#}
#------------------------------------------------------------------------------------


# Install given package name using brew
fetch_brew_dependency() {
  FORMULA_NAME=$1

  echo "Fetch brew dependency: '$FORMULA_NAME'."

  if brew ls --versions "$FORMULA_NAME" > /dev/null; then
    echo "Dependency '$FORMULA_NAME' is already installed, continuing..."
  else
    echo "Dependency '$FORMULA_NAME' is not installed, installing via homebrew..."
    brew install "$FORMULA_NAME"
  fi
}

#------------------------------------------------------------------------------------

verify_libs_folder_exists() {
  if [ ! -d "libs" ]; then
    mkdir libs
  fi
}

# if third-party folder doesn't exist we create it
verify_third_party_folder_exists() {
  verify_libs_folder_exists

  if [ ! -d "libs/third-party" ]; then
    mkdir libs/third-party
  fi
}

verify_frameworks_folder_exists() {
  verify_libs_folder_exists

  pushd libs || exit
    if [ ! -d "Frameworks" ]; then
      mkdir Frameworks
    fi
  popd || exit
}

fetch_third_party_lib_sdl() {
  verify_third_party_folder_exists

  # shellcheck disable=SC2164
  pushd libs/third-party
    if [ ! -d "SDL" ] ; then
      echo "Fetching SDL..."

      # Download the SDL2 source zip file
      wget https://www.libsdl.org/release/SDL2-2.0.9.zip

      # Unzip the file into the current folder
      unzip -q SDL2-2.0.9.zip

      # Rename the SDL2-2.0.9 folder to SDL
      mv SDL2-2.0.9 SDL

      # Clean up by deleting the zip file that we downloaded.
      rm SDL2-2.0.9.zip
    else
      echo "SDL library already exists in third party folder."
    fi
  # shellcheck disable=SC2164
  popd
}

fetch_third_party_lib_sdl_image() {
  verify_third_party_folder_exists
  echo "Fetching SDL Image..."
    # shellcheck disable=SC2164
    pushd libs/third-party
      if [ ! -d "SDL2_image" ] ; then
        echo "Fetching SDL Image..."
        wget https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.4.zip
        unzip -q SDL2_image-2.0.4.zip
        mv SDL2_image-2.0.4 SDL2_image
        rm SDL2_image-2.0.4.zip
      fi
    # shellcheck disable=SC2164
    popd
}

fetch_third_party_lib_glm() {
  verify_third_party_folder_exists

  # shellcheck disable=SC2164
  pushd libs/third-party
    if [ ! -d "glm" ] ; then
      echo "Fetching GLM"
      wget https://github.com/g-truc/glm/releases/download/0.9.9.3/glm-0.9.9.3.zip
      unzip -q glm-0.9.9.3.zip
      rm glm-0.9.9.3.zip
    fi
  # shellcheck disable=SC2164
  popd
}

fetch_third_party_lib_tiny_obj_loader() {
  verify_third_party_folder_exists

  # shellcheck disable=SC2164
  pushd libs/third-party
      if [ ! -d "tiny-obj-loader" ] ; then
        echo "Fetching Tiny OBJ Loader"
        wget https://github.com/tinyobjloader/tinyobjloader/archive/refs/tags/v1.0.6.zip
        unzip -q v1.0.6.zip
        rm v1.0.6.zip
        mv tinyobjloader-1.0.6 tiny-obj-loader
      fi
    # shellcheck disable=SC2164
    popd
}

fetch_third_party_lib_imgui() {
  verify_third_party_folder_exists

  # shellcheck disable=SC2164
  pushd libs/third-party
    if [ ! -d "imgui" ] ; then
      echo "Fetching imgui"
      wget https://github.com/ocornut/imgui/archive/refs/tags/v1.90.9-docking.zip
      unzip -q v1.90.9-docking.zip
      rm v1.90.9-docking.zip
      mv imgui-1.90.9-docking imgui
    fi
  # shellcheck disable=SC2164
  popd
}

fetch_third_party_lib_tracy() {
  verify_third_party_folder_exists

  # shellcheck disable=SC2164
  pushd libs/third-party
    if [ ! -d "tracy" ] ; then
      echo "Fetching tracy"
      wget https://github.com/wolfpld/tracy/archive/refs/tags/v0.10.zip
      unzip -q v0.10.zip
      rm v0.10.zip
      mv tracy-0.10 tracy
    fi

    if [ -d "tracy" ] ; then
      echo "Building Tracy Profiler Server ..."

      # require freetype2 and capstone and sdl2
      pushd tracy/profiler/build/unix || exit
        make
      popd || exit
    fi

  # shellcheck disable=SC2164
  popd
}

# for ECS imlementation
fetch_third_party_lib_entt()
{
  verify_third_party_folder_exists

  # shellcheck disable=SC2164
    pushd libs/third-party
      if [ ! -d "entt" ] ; then
        echo "Fetching entt"
        wget https://github.com/skypjack/entt/archive/refs/tags/v3.13.2.zip
        unzip -q v3.13.2.zip
        rm v3.13.2.zip
        mv entt-3.13.2 entt
      fi

      if [ -d "entt" ] ; then
        echo "Building Entt documentation ..."

        # require freetype2 and capstone and sdl2
        pushd entt/build || exit
          cmake .. -DENTT_BUILD_DOCS=ON
          make
        popd || exit
      fi

    # shellcheck disable=SC2164
    popd
}

#------------------------------------------------------------------------------------

# MacOS Lib's
fetch_framework_sdl2() {
  verify_frameworks_folder_exists

  # shellcheck disable=SC2164
  pushd libs/Frameworks
    if [ ! -d "SDL2.framework" ]; then
      # Download the .dmg file from the SDL2 download site.
      wget https://www.libsdl.org/release/SDL2-2.0.9.dmg

      echo "Mounting DMG file ..."
      hdiutil attach SDL2-2.0.9.dmg

      echo "Copying SDL2.framework from DMG file into the current folder ..."
      cp -R /Volumes/SDL2/SDL2.framework .

      echo "Unmounting DMG file ..."
      hdiutil detach /Volumes/SDL2

      echo "Deleting DMG file ..."
      rm SDL2-2.0.9.dmg

      # Navigate into the SDL2.framework folder.
      # shellcheck disable=SC2164
      pushd SDL2.framework
          echo "Code signing SDL2.framework ..."
          codesign -f -s - SDL2
      # shellcheck disable=SC2164
      popd
    else
      echo "SDL 2 Frameworks already exists..."
    fi

  # shellcheck disable=SC2164
  popd
}

fetch_framework_sdl2_image() {
    verify_frameworks_folder_exists

    # shellcheck disable=SC2164
    pushd libs/Frameworks
        if [ ! -d "SDL2_image.framework" ]; then
            echo "Fetching SDL2_image framework from: https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.4.dmg"
            wget https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.4.dmg

            echo "Attaching DMG file ..."
            hdiutil attach SDL2_image-2.0.4.dmg

            echo "Copying SDL2_image.framework from DMG file ..."
            cp -R /Volumes/SDL2_image/SDL2_image.framework .

            echo "Detaching DMG file ..."
            hdiutil detach /Volumes/SDL2_image
            rm SDL2_image-2.0.4.dmg

            # We need to code sign a couple of binaries to avoid Xcode errors
            # shellcheck disable=SC2164
            pushd SDL2_image.framework/Versions/A/Frameworks/webp.framework
                echo "Code signing SDL2_image.framework / Frameworks / webp.framework ..."
                codesign -f -s - webp
            # shellcheck disable=SC2164
            popd

            # shellcheck disable=SC2164
            pushd SDL2_image.framework
                echo "Code signing SDL2_image.framework ..."
                codesign -f -s - SDL2_image
            # shellcheck disable=SC2164
            popd
        fi
    # shellcheck disable=SC2164
    popd
}

#------------------------------------------------------------------------------------

verify_build_folder_exists() {
  echo "Checking for build folder"
  platform=$1

  if [ ! -d "builds" ]; then
    mkdir builds
  fi

  pushd builds || exit
    if [ ! -d "$platform" ]; then
      mkdir "$platform"
    fi
  popd || exit
}

clean_ninja_folder() { #clean_build_folder
  platform=$1

  echo "Cleaning build folder"

  rm -r installer/platforms/"$platform"/build/*
}

clean_build_folder() { #clean_out_folder
  platform=$1

  echo "Deleting files from previous build..."

  # Find and remove all files and directories except the one to keep
  find builds/"$platform"/ -mindepth 1 ! -name imgui.ini -exec rm -rf {} +

  # Print a message indicating the cleanup was successful
  if [ $? -eq 0 ]; then
      echo "Cleanup completed successfully."
  else
      echo "Cleanup failed."
  fi
#  rm -r builds/"$platform"/*
}


#------------------------------------------------------------------------------------