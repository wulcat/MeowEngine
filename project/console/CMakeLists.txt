cmake_minimum_required(VERSION 3.4.1)
project(physicat)

set(CMAKE_OSX_ARCHITECTURES "x86_64")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fexceptions")

set(THIRD_PARTY_DIR "../../third-party")
set(MAIN_SOURCE_DIR "../main/src")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out)

include_directories(${THIRD_PARTY_DIR}/SDL/include)
include_directories(${THIRD_PARTY_DIR}/glm)
include_directories(${THIRD_PARTY_DIR}/tiny-obj-loader)
include_directories(${THIRD_PARTY_DIR}/SDL2_image)

# glob is not good idea
file(GLOB_RECURSE CPP_HEADERS ${MAIN_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE CPP_SOURCES ${MAIN_SOURCE_DIR}/*.cpp)

include_directories(../main/src/include/bitmap.hpp)
#include_directories(console ${CPP_HEADERS} ${CPP_SOURCES})

#message(${CMAKE_SOURCE_DIR})
# Collect all directories under the include directory
file(GLOB_RECURSE ALL_ENTRIES "../main/*")

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

# Print the list of directories (for debugging purposes)
message("Directories found in ${DIRECTORY_TO_SEARCH}:")
foreach(DIR ${ALL_DIRECTORIES})
     message("${DIR}")
     include_directories(${DIR})
endforeach()
#message(${HEADER_DIRECTORIES})

## Include each directory
#foreach(HEADER_DIR ${HEADER_DIRECTORIES})
##     include_directories(${MAIN_SOURCE_DIR}/${HEADER_DIR})
#message(${HEADER_DIR})
#     message(${MAIN_SOURCE_DIR}/${HEADER_DIR})
#endforeach()

add_executable(
    console
    ${CPP_HEADERS}
    ${CPP_SOURCES}
        ../main/src/core/wrappers/glm_wrapper.cpp ../main/src/core/wrappers/glm_wrapper.hpp ../main/src/core/assets/mesh/vertex.cpp ../main/src/core/assets/mesh/vertex.hpp ../main/src/core/assets/mesh/mesh.cpp ../main/src/core/assets/mesh/mesh.hpp ../main/src/core/assets/assets.cpp ../main/src/core/assets/assets.hpp ../main/src/application/opengl/opengl_pipeline.cpp ../main/src/application/opengl/opengl_pipeline.hpp ../main/src/application/opengl/opengl_mesh.cpp ../main/src/application/opengl/opengl_mesh.hpp ../main/src/core/cameras/perspective_camera.cpp ../main/src/core/cameras/perspective_camera.hpp ../main/src/application/opengl/opengl_texture.cpp ../main/src/include/opengl_texture.hpp)

# This is to ensure executable knows how to attach frameworks or other things to itself
set_target_properties(
    console
    PROPERTIES
    LINK_FLAGS
    "-F../Frameworks -framework SDL2 -framework SDL2_image -framework OpenGL"
)

# Executes after build to properly link sdk framework in executable file
add_custom_command(
    TARGET console
    POST_BUILD
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND ./cmake-post-build.sh
)