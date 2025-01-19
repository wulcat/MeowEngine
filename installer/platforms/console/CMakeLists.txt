cmake_minimum_required(VERSION 3.4.1)
project(MeowEngine)

set(CMAKE_OSX_ARCHITECTURES "x86_64")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fexceptions")
#set(CMAKE_C_FLAGS "-Wall -DTRACY_ENABLE")
#set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVK_PROTOTYPES")
set(THIRD_PARTY_DIR "../../../libs/third-party")
set(MAIN_SOURCE_DIR "../../../engine/source")
set(INSTALLER_DIR "../../console")
set(LIBRARY_DIR "../../../../libs")

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY out)
# Set the output directories
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Optionally set custom directories for Debug and Release builds
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../../../../builds/console)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})

#
find_package(OpenCL REQUIRED) # required for imgui
find_package(OpenGL REQUIRED) # required for tracy & native menu options with bridge wrapper
find_package(SDL2 REQUIRED) # required for native menu options with bridge wrapper


# Include directories for intellisense
include_directories(${THIRD_PARTY_DIR}/SDL/include)
include_directories(${THIRD_PARTY_DIR}/glm)
include_directories(${THIRD_PARTY_DIR}/tiny-obj-loader)
include_directories(${THIRD_PARTY_DIR}/SDL2_image)
include_directories(${THIRD_PARTY_DIR}/concurrentqueue)
include_directories(${THIRD_PARTY_DIR}/imgui)
include_directories(${THIRD_PARTY_DIR}/imgui/backends)
include_directories(${THIRD_PARTY_DIR}/tracy/public/tracy)
include_directories(${THIRD_PARTY_DIR}/entt/single_include)
include_directories(${THIRD_PARTY_DIR}/physx/physx/include)

# glob is not good idea
file(GLOB_RECURSE CPP_HEADERS ${MAIN_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE CPP_SOURCES ${MAIN_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE M_SOURCES ${MAIN_SOURCE_DIR}/*.m)

#include_directories(../main/src/include/bitmap.hpp)

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

add_executable(
     MeowEngine
     ${THIRD_PARTY_DIR}/tracy/public/TracyClient.cpp
     ${THIRD_PARTY_DIR}/concurrentqueue/concurrentqueue.h
     ${THIRD_PARTY_DIR}/imgui/imgui.cpp ${THIRD_PARTY_DIR}/imgui/imgui_draw.cpp ${THIRD_PARTY_DIR}/imgui/imgui_tables.cpp ${THIRD_PARTY_DIR}/imgui/imgui_widgets.cpp ${THIRD_PARTY_DIR}/imgui/imgui_demo.cpp ${THIRD_PARTY_DIR}/imgui/backends/imgui_impl_sdl2.cpp ${THIRD_PARTY_DIR}/imgui/backends/imgui_impl_opengl3.cpp
     ${CPP_HEADERS}
     ${CPP_SOURCES}
     ${M_SOURCES}
)

# Link profiler with console executable

add_library(
   TracyClient STATIC
        ${THIRD_PARTY_DIR}/tracy/public/TracyClient.cpp
        ${THIRD_PARTY_DIR}/tracy/public/tracy/TracyOpenCL.hpp
#        ${THIRD_PARTY_DIR}/tracy/public/tracy/TracyOpenGL.hpp
)
#target_include_directories(TracyClient PUBLIC ../../public/tracy)
target_compile_definitions(TracyClient PUBLIC TRACY_ENABLE=1)
target_link_libraries(MeowEngine PUBLIC OpenCL::OpenCL TracyClient ${CMAKE_DL_LIBS})
#target_link_libraries(MeowEngine PUBLIC OpenGL::OpenGL TracyClient ${CMAKE_DL_LIBS})

# Link native menu option for mac
#include_directories(${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})

target_link_libraries(MeowEngine PUBLIC
   ${SDL2_LIBRARIES}
   ${OPENGL_LIBRARIES} "-framework Cocoa"
)

# BUG: some weird issue here. PHYSX_LIB is somehow already set to a physic common lib path.
find_library(PHYSX_LIBRARY NAMES libPhysX_static_64.a PATHS ${THIRD_PARTY_DIR}/physx/physx/bin/linux.clang/release)
find_library(PHYSX_COMMON_LIB NAMES libPhysXCommon_static_64.a PATHS ${THIRD_PARTY_DIR}/physx/physx/bin/linux.clang/release)
find_library(PHYSX_FOUNDATION_LIB NAMES libPhysXFoundation_static_64.a PATHS ${THIRD_PARTY_DIR}/physx/physx/bin/linux.clang/release)
find_library(PHYSX_EXTENSIONS_LIB NAMES libPhysXExtensions_static_64.a PATHS ${THIRD_PARTY_DIR}/physx/physx/bin/linux.clang/release)

target_link_libraries(MeowEngine PUBLIC
     ${PHYSX_LIBRARY}
     ${PHYSX_COMMON_LIB}
     ${PHYSX_FOUNDATION_LIB}
     ${PHYSX_EXTENSIONS_LIB}
)


# This is to ensure executable knows how to attach frameworks or other things to itself
set_target_properties(
        MeowEngine
    PROPERTIES
    LINK_FLAGS
    "-F${LIBRARY_DIR}/Frameworks -framework SDL2 -framework SDL2_image -framework OpenGL"
)

#echo "Current working directory: $(pwd)"

# Executes after build to properly link sdk framework in executable file
add_custom_command(
    TARGET MeowEngine
    POST_BUILD
    WORKING_DIRECTORY ${INSTALLER_DIR}
    COMMAND ./cmake-post-build.sh -p console
)