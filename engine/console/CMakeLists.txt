cmake_minimum_required(VERSION 3.4.1)
project(physicat)

set(CMAKE_OSX_ARCHITECTURES "x86_64")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fexceptions")
#set(CMAKE_C_FLAGS "-Wall -DTRACY_ENABLE")
#set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVK_PROTOTYPES")
set(THIRD_PARTY_DIR "../../third-party")
set(MAIN_SOURCE_DIR "../main/src")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out)

#
find_package(OpenCL REQUIRED) # required for imgui
find_package(OpenGL REQUIRED) # required for tracy & native menu options with bridge wrapper
find_package(SDL2 REQUIRED) # required for native menu options with bridge wrapper


# Include directories for intellisense
include_directories(${THIRD_PARTY_DIR}/SDL/include)
include_directories(${THIRD_PARTY_DIR}/glm)
include_directories(${THIRD_PARTY_DIR}/tiny-obj-loader)
include_directories(${THIRD_PARTY_DIR}/SDL2_image)
include_directories(${THIRD_PARTY_DIR}/imgui)
include_directories(${THIRD_PARTY_DIR}/imgui/backends)
include_directories(${THIRD_PARTY_DIR}/tracy/public/tracy)

# glob is not good idea
file(GLOB_RECURSE CPP_HEADERS ${MAIN_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE CPP_SOURCES ${MAIN_SOURCE_DIR}/*.cpp)

#include_directories(../main/src/include/bitmap.hpp)

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

message("Directories found in ${DIRECTORY_TO_SEARCH}:")
foreach(DIR ${ALL_DIRECTORIES})
     message("${DIR}")
     include_directories(${DIR})
endforeach()

add_executable(
    console
    ${THIRD_PARTY_DIR}/tracy/public/TracyClient.cpp
    ${THIRD_PARTY_DIR}/imgui/imgui.cpp ${THIRD_PARTY_DIR}/imgui/imgui_draw.cpp ${THIRD_PARTY_DIR}/imgui/imgui_tables.cpp ${THIRD_PARTY_DIR}/imgui/imgui_widgets.cpp ${THIRD_PARTY_DIR}/imgui/imgui_demo.cpp ${THIRD_PARTY_DIR}/imgui/backends/imgui_impl_sdl2.cpp ${THIRD_PARTY_DIR}/imgui/backends/imgui_impl_opengl3.cpp
    ${CPP_HEADERS}
    ${CPP_SOURCES}
        ../main/src/wrappers/glm_wrapper.hpp ../main/src/core/assets/mesh/vertex.cpp ../main/src/core/assets/mesh/vertex.hpp ../main/src/core/assets/mesh/mesh.cpp ../main/src/core/assets/mesh/mesh.hpp ../main/src/core/assets/assets.cpp ../main/src/core/assets/assets.hpp ../main/src/application/graphics/opengl/opengl_mesh.cpp ../main/src/application/graphics/opengl/opengl_mesh.hpp ../main/src/core/assets/cameras/perspective_camera.cpp ../main/src/core/assets/cameras/perspective_camera.hpp ../main/src/application/graphics/opengl/opengl_texture.cpp ../main/src/application/graphics/opengl/opengl_texture.hpp ../main/src/core/assets/asset_inventory.cpp ../main/src/core/assets/asset_inventory.hpp ../main/src/core/assets/asset_manager.cpp ../main/src/core/assets/asset_manager.hpp ../main/src/application/graphics/opengl/opengl_asset_manager.cpp ../main/src/application/graphics/opengl/opengl_asset_manager.hpp ../main/src/core/assets/mesh/static_mesh_instance.cpp ../main/src/core/assets/mesh/static_mesh_instance.hpp ../main/src/application/graphics/renderer.cpp ../main/src/application/graphics/renderer.hpp ../main/src/application/graphics/opengl/opengl_renderer.cpp ../main/src/application/graphics/opengl/opengl_renderer.hpp ../main/src/core/scene/scene.hpp ../main/src/core/scene/main_scene.cpp ../main/src/core/scene/main_scene.hpp ../main/src/application/graphics/opengl/pipelines/opengl_line_pipeline.cpp ../main/src/application/graphics/opengl/pipelines/opengl_line_pipeline.hpp ../main/src/application/graphics/opengl/pipelines/opengl_mesh_pipeline.cpp ../main/src/application/graphics/opengl/pipelines/opengl_mesh_pipeline.hpp ../main/src/core/assets/lifeobject/life_object.cpp ../main/src/core/assets/lifeobject/life_object.hpp ../main/src/core/assets/lifeobject/components/renders/render_component_base.cpp ../main/src/core/assets/lifeobject/components/renders/render_component_base.hpp ../main/src/core/assets/lifeobject/components/physics/physics_component_base.cpp ../main/src/core/assets/lifeobject/components/physics/physics_component_base.hpp ../main/src/core/assets/lifeobject/components/component_base.hpp ../main/src/core/assets/lifeobject/components/renders/mesh_render_component.cpp ../main/src/core/assets/lifeobject/components/renders/mesh_render_component.hpp ../main/src/core/assets/lifeobject/components/renders/line_render_component.cpp ../main/src/core/assets/lifeobject/components/renders/line_render_component.hpp ../main/src/application/graphics/opengl/pipelines/opengl_pipeline_base.hpp ../main/src/application/graphics/opengl/pipelines/opengl_pipeline_base.cpp ../main/src/core/assets/lifeobject/components/core/transform2d_component.cpp ../main/src/core/assets/lifeobject/components/core/transform2d_component.hpp ../main/src/core/assets/lifeobject/components/core/transform3d_component.cpp ../main/src/core/assets/lifeobject/components/core/transform3d_component.hpp ../main/src/core/assets/lifeobject/components/core/transform_component_base.cpp ../main/src/core/assets/lifeobject/components/core/transform_component_base.hpp ../main/src/application/graphics/opengl/pipelines/opengl_grid_pipeline.cpp ../main/src/application/graphics/opengl/pipelines/opengl_grid_pipeline.hpp ../main/src/application/editor/window_size.hpp ../main/src/core/assets/cameras/camera_controller.cpp ../main/src/core/assets/cameras/camera_controller.hpp ../main/src/wrappers/imgui_wrapper.hpp ../main/src/application/graphics/imgui/imgui_renderer.cpp ../main/src/application/graphics/imgui/imgui_renderer.hpp ../main/src/wrappers/tracy_wrapper.hpp ../main/src/wrappers/bridge_wrapper.hpp ../main/src/application/editor/mac_bridge.m ../main/src/application/graphics/opengl/opengl_framebuffer.cpp ../main/src/application/graphics/opengl/opengl_framebuffer.hpp ../main/src/application/input/input_manager.cpp ../main/src/application/input/input_manager.hpp)

# Link profiler with console executable

add_library(
   TracyClient STATIC
        ${THIRD_PARTY_DIR}/tracy/public/TracyClient.cpp
        ${THIRD_PARTY_DIR}/tracy/public/tracy/TracyOpenCL.hpp
#        ${THIRD_PARTY_DIR}/tracy/public/tracy/TracyOpenGL.hpp
)
#target_include_directories(TracyClient PUBLIC ../../public/tracy)
target_compile_definitions(TracyClient PUBLIC TRACY_ENABLE=1)
target_link_libraries(console PUBLIC OpenCL::OpenCL TracyClient ${CMAKE_DL_LIBS})
#target_link_libraries(console PUBLIC OpenGL::OpenGL TracyClient ${CMAKE_DL_LIBS})

# Link native menu option for mac
#include_directories(${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})
target_link_libraries(console PUBLIC ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} "-framework Cocoa")

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