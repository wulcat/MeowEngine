
cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_OSX_ARCHITECTURES "x86_64")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fexceptions")

set(THIRD_PARTY_DIR "../../third-party")
set(MAIN_SOURCE_DIR "../main/src")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out)

include_directories(${THIRD_PARTY_DIR}/SDL/include)

file(GLOB_RECURSE CPP_HEADERS ${MAIN_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE CPP_SOURCES ${MAIN_SOURCE_DIR}/*.cpp)

add_executable(
    engine-console
    ${CPP_HEADERS}
    ${CPP_SOURCES}
)

#IF(APPLE)
#    SET(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "Build architectures for Mac OS X" FORCE)
#ENDIF(APPLE)

# This is to ensure executable knows how to attach frameworks or other things to itself
set_target_properties(
    engine-console
    PROPERTIES
    LINK_FLAGS
    "-F../Frameworks -framework SDL2 -framework OpenGL"
)

add_custom_command(
    TARGET engine-console
    POST_BUILD
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND ./cmake-post-build.sh
)