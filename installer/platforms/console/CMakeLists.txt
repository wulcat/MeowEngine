cmake_minimum_required(VERSION 3.4.1)
project(physicat)

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
include_directories(${THIRD_PARTY_DIR}/imgui)
include_directories(${THIRD_PARTY_DIR}/imgui/backends)
include_directories(${THIRD_PARTY_DIR}/tracy/public/tracy)

# glob is not good idea
file(GLOB_RECURSE CPP_HEADERS ${MAIN_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE CPP_SOURCES ${MAIN_SOURCE_DIR}/*.cpp)

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
    Physicat
    ${THIRD_PARTY_DIR}/tracy/public/TracyClient.cpp
    ${THIRD_PARTY_DIR}/imgui/imgui.cpp ${THIRD_PARTY_DIR}/imgui/imgui_draw.cpp ${THIRD_PARTY_DIR}/imgui/imgui_tables.cpp ${THIRD_PARTY_DIR}/imgui/imgui_widgets.cpp ${THIRD_PARTY_DIR}/imgui/imgui_demo.cpp ${THIRD_PARTY_DIR}/imgui/backends/imgui_impl_sdl2.cpp ${THIRD_PARTY_DIR}/imgui/backends/imgui_impl_opengl3.cpp
    ${CPP_HEADERS}
    ${CPP_SOURCES}
        ../../../engine/source/editor/wrappers/glm_wrapper.hpp ../../../engine/source/core/assets/mesh/vertex.cpp ../../../engine/source/core/assets/mesh/vertex.hpp ../../../engine/source/core/assets/mesh/mesh.cpp ../../../engine/source/core/assets/mesh/mesh.hpp ../../../engine/source/core/assets/assets.cpp ../../../engine/source/core/assets/assets.hpp ../../../engine/source/editor/application/graphics/opengl/opengl_mesh.cpp ../../../engine/source/editor/application/graphics/opengl/opengl_mesh.hpp ../../../engine/source/core/assets/cameras/perspective_camera.cpp ../../../engine/source/core/assets/cameras/perspective_camera.hpp ../../../engine/source/editor/application/graphics/opengl/opengl_texture.cpp ../../../engine/source/editor/application/graphics/opengl/opengl_texture.hpp ../../../engine/source/core/assets/asset_inventory.cpp ../../../engine/source/core/assets/asset_inventory.hpp ../../../engine/source/core/assets/asset_manager.cpp ../../../engine/source/core/assets/asset_manager.hpp ../../../engine/source/editor/application/graphics/opengl/opengl_asset_manager.cpp ../../../engine/source/editor/application/graphics/opengl/opengl_asset_manager.hpp ../../../engine/source/core/assets/mesh/static_mesh_instance.cpp ../../../engine/source/core/assets/mesh/static_mesh_instance.hpp ../../../engine/source/editor/application/graphics/renderer.cpp ../../../engine/source/editor/application/graphics/renderer.hpp ../../../engine/source/editor/application/graphics/opengl/opengl_renderer.cpp ../../../engine/source/editor/application/graphics/opengl/opengl_renderer.hpp ../../../engine/source/core/scene/scene.hpp ../../../engine/source/core/scene/main_scene.cpp ../../../engine/source/core/scene/main_scene.hpp ../../../engine/source/editor/application/graphics/opengl/pipelines/opengl_line_pipeline.cpp ../../../engine/source/editor/application/graphics/opengl/pipelines/opengl_line_pipeline.hpp ../../../engine/source/editor/application/graphics/opengl/pipelines/opengl_mesh_pipeline.cpp ../../../engine/source/editor/application/graphics/opengl/pipelines/opengl_mesh_pipeline.hpp ../../../engine/source/core/assets/lifeobject/life_object.cpp ../../../engine/source/core/assets/lifeobject/life_object.hpp ../../../engine/source/core/assets/lifeobject/components/renders/render_component_base.cpp ../../../engine/source/core/assets/lifeobject/components/renders/render_component_base.hpp ../../../engine/source/core/assets/lifeobject/components/physics/physics_component_base.cpp ../../../engine/source/core/assets/lifeobject/components/physics/physics_component_base.hpp ../../../engine/source/core/assets/lifeobject/components/component_base.hpp ../../../engine/source/core/assets/lifeobject/components/renders/mesh_render_component.cpp ../../../engine/source/core/assets/lifeobject/components/renders/mesh_render_component.hpp ../../../engine/source/core/assets/lifeobject/components/renders/line_render_component.cpp ../../../engine/source/core/assets/lifeobject/components/renders/line_render_component.hpp ../../../engine/source/editor/application/graphics/opengl/pipelines/opengl_pipeline_base.hpp ../../../engine/source/editor/application/graphics/opengl/pipelines/opengl_pipeline_base.cpp ../../../engine/source/core/assets/lifeobject/components/core/transform2d_component.cpp ../../../engine/source/core/assets/lifeobject/components/core/transform2d_component.hpp ../../../engine/source/core/assets/lifeobject/components/core/transform3d_component.cpp ../../../engine/source/core/assets/lifeobject/components/core/transform3d_component.hpp ../../../engine/source/core/assets/lifeobject/components/core/transform_component_base.cpp ../../../engine/source/core/assets/lifeobject/components/core/transform_component_base.hpp ../../../engine/source/editor/application/graphics/opengl/pipelines/opengl_grid_pipeline.cpp ../../../engine/source/editor/application/graphics/opengl/pipelines/opengl_grid_pipeline.hpp ../../../engine/source/editor/application/editor/window_size.hpp ../../../engine/source/core/assets/cameras/camera_controller.cpp ../../../engine/source/core/assets/cameras/camera_controller.hpp ../../../engine/source/editor/wrappers/imgui_wrapper.hpp ../../../engine/source/editor/application/graphics/imgui/imgui_renderer.cpp ../../../engine/source/editor/application/graphics/imgui/imgui_renderer.hpp ../../../engine/source/editor/wrappers/tracy_wrapper.hpp ../../../engine/source/editor/wrappers/bridge_wrapper.hpp ../../../engine/source/editor/application/editor/mac_bridge.m ../../../engine/source/editor/application/graphics/opengl/opengl_framebuffer.cpp ../../../engine/source/editor/application/graphics/opengl/opengl_framebuffer.hpp ../../../engine/source/editor/application/input/input_manager.cpp ../../../engine/source/editor/application/input/input_manager.hpp ../../../engine/source/editor/application/graphics/imgui/panels/imgui_lifeobject_list_panel.cpp ../../../engine/source/editor/application/graphics/imgui/panels/imgui_lifeobject_list_panel.hpp ../../../engine/source/editor/application/graphics/imgui/panels/imgui_edit_panel.cpp ../../../engine/source/editor/application/graphics/imgui/panels/imgui_edit_panel.hpp ../../../engine/source/core/math/vector3.cpp ../../../engine/source/core/math/vector3.hpp ../../../engine/source/core/math/math_wrapper.hpp ../../../engine/source/editor/application/graphics/imgui/panels/imgui_log_panel.cpp ../../../engine/source/editor/application/graphics/imgui/panels/imgui_log_panel.hpp ../../../engine/source/editor/application/graphics/imgui/panels/imgui_world_render_panel.cpp ../../../engine/source/editor/application/graphics/imgui/panels/imgui_world_render_panel.hpp ../../../engine/source/editor/application/graphics/imgui/imgui_docker_space.cpp ../../../engine/source/editor/application/graphics/imgui/imgui_docker_space.hpp)

# Link profiler with console executable

add_library(
   TracyClient STATIC
        ${THIRD_PARTY_DIR}/tracy/public/TracyClient.cpp
        ${THIRD_PARTY_DIR}/tracy/public/tracy/TracyOpenCL.hpp
#        ${THIRD_PARTY_DIR}/tracy/public/tracy/TracyOpenGL.hpp
)
#target_include_directories(TracyClient PUBLIC ../../public/tracy)
target_compile_definitions(TracyClient PUBLIC TRACY_ENABLE=1)
target_link_libraries(Physicat PUBLIC OpenCL::OpenCL TracyClient ${CMAKE_DL_LIBS})
#target_link_libraries(Physicat PUBLIC OpenGL::OpenGL TracyClient ${CMAKE_DL_LIBS})

# Link native menu option for mac
#include_directories(${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})
target_link_libraries(Physicat PUBLIC ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} "-framework Cocoa")

# This is to ensure executable knows how to attach frameworks or other things to itself
set_target_properties(
        Physicat
    PROPERTIES
    LINK_FLAGS
    "-F${LIBRARY_DIR}/Frameworks -framework SDL2 -framework SDL2_image -framework OpenGL"
)

#echo "Current working directory: $(pwd)"

# Executes after build to properly link sdk framework in executable file
add_custom_command(
    TARGET Physicat
    POST_BUILD
    WORKING_DIRECTORY ${INSTALLER_DIR}
    COMMAND ./cmake-post-build.sh -p console
)