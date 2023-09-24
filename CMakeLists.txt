cmake_minimum_required(VERSION 3.12)
project(voxel-factory-extensions)

# Set the C++ standard for your project
set(CMAKE_CXX_STANDARD 20)

# Generate compile_commands.json for IDEs 
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set default build type to Debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Fetch and include godot-cpp library
include(FetchContent)
FetchContent_Declare(
    godot-cpp
    GIT_REPOSITORY https://github.com/godotengine/godot-cpp.git
    GIT_TAG        4.1
)
FetchContent_MakeAvailable(godot-cpp)

# Enable optimiations for godot-cpp in debug mode
if (CMAKE_BUILD_TYPE MATCHES "Debug" AND CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(godot-cpp PRIVATE "-O3")
endif()

# Add voxelfactory library
add_subdirectory("lib/voxelfactory")

# Set extension output directory
set(GODOT_EXT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/game/bin")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${GODOT_EXT_DIR})
if (WIN32)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${GODOT_EXT_DIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${GODOT_EXT_DIR})
    
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${GODOT_EXT_DIR})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${GODOT_EXT_DIR})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${GODOT_EXT_DIR})
    
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${GODOT_EXT_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${GODOT_EXT_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${GODOT_EXT_DIR})
endif()

# Add extension subdirectories
file(GLOB EXT_DIRS "extensions/*")
foreach(EXT_DIR ${EXT_DIRS})
    add_subdirectory(${EXT_DIR})
endforeach()