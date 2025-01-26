cmake_minimum_required(VERSION 3.4.1)
project(MeowEngine)

set(CMAKE_OSX_ARCHITECTURES "x86_64")
# We are using C++ 17 and will make use of C++ exceptions.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fexceptions")

set(THIRD_PARTY_DIR "../../../libs/third-party")
set(MAIN_SOURCE_DIR "../../../engine/source")
#set(INSTALLER_DIR "../../web")
#set(LIBRARY_DIR "../../../../libs")

#find_package(OpenCL REQUIRED) # required for imgui

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY out)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../../../../builds/web)

include_directories(${THIRD_PARTY_DIR}/SDL/include)
include_directories(${THIRD_PARTY_DIR}/glm)
include_directories(${THIRD_PARTY_DIR}/tiny-obj-loader)
include_directories(${THIRD_PARTY_DIR}/SDL2_image)
include_directories(${THIRD_PARTY_DIR}/concurrentqueue)
include_directories(${THIRD_PARTY_DIR}/imgui)
include_directories(${THIRD_PARTY_DIR}/imgui/backends)
include_directories(${THIRD_PARTY_DIR}/tracy/public/tracy) # need to remove this
include_directories(${THIRD_PARTY_DIR}/entt/single_include)
include_directories(${THIRD_PARTY_DIR}/physx/physx/include)

file(GLOB_RECURSE CPP_HEADERS ${MAIN_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE CPP_SOURCES ${MAIN_SOURCE_DIR}/*.cpp)

# Collect all directories under the include directory
file(GLOB_RECURSE ALL_ENTRIES "${MAIN_SOURCE_DIR}/*")

# Extract directories from the list of entries
set(ALL_DIRECTORIES "")
foreach(ENTRY ${ALL_ENTRIES})
    get_filename_component(DIR ${ENTRY} DIRECTORY)
    if(IS_DIRECTORY ${DIR})
        list(APPEND ALL_DIRECTORIES ${DIR})
    endif()
endforeach()

# Remove duplicates from the list of directories
list(REMOVE_DUPLICATES ALL_DIRECTORIES)

message("Directories found in ${DIRECTORY_TO_SEARCH}:")
foreach(DIR ${ALL_DIRECTORIES})
    message("${DIR}")
    include_directories(${DIR})
endforeach()

# The 'web' executable is the output target for our Emscripten application and is why we end up with 'web.js' in the output folder. Note that it will use the C++ source code from the shared 'main' folder through the collated sources.
add_executable(
    web
    ${THIRD_PARTY_DIR}/imgui/imgui.cpp ${THIRD_PARTY_DIR}/imgui/imgui_draw.cpp ${THIRD_PARTY_DIR}/imgui/imgui_tables.cpp ${THIRD_PARTY_DIR}/imgui/imgui_widgets.cpp ${THIRD_PARTY_DIR}/imgui/imgui_demo.cpp ${THIRD_PARTY_DIR}/imgui/backends/imgui_impl_sdl2.cpp ${THIRD_PARTY_DIR}/imgui/backends/imgui_impl_opengl3.cpp
    ${CPP_HEADERS}
    ${CPP_SOURCES}
)


# These properties specify what kind of Emscripten build to perform and are assigned to our 'a-simple-triangle' executable target.
set_target_properties(
    web
    PROPERTIES
    LINK_FLAGS
    "-O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=\"['png', 'jpg']\" -s USE_WEBGL2=1 --preload-file ../../../../engine/assets@/assets"
)

# When the build is complete we will run the 'cmake-post-build.sh' script.
add_custom_command(
    TARGET web
    POST_BUILD
    WORKING_DIRECTORY ../../web
    COMMAND ./cmake-post-build.sh
)