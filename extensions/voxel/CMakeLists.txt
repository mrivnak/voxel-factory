project(gdvoxel)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES
    src/gdvoxel.cpp
    src/register_types.cpp
)

add_library(gdvoxel SHARED ${SOURCES})

target_include_directories(gdvoxel PUBLIC
    ./include
)

set_target_properties(gdvoxel PROPERTIES
    OUTPUT_NAME "gdvoxel.${CMAKE_SYSTEM_NAME}.${CMAKE_BUILD_TYPE}.${CMAKE_SYSTEM_PROCESSOR}"
)

# Set symbol visibility to hidden for GCC and Clang
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(gdvoxel PRIVATE "-fvisibility=hidden")
    target_compile_options(gdvoxel PRIVATE "-fvisibility-inlines-hidden")
endif()

target_link_libraries(gdvoxel PRIVATE
    godot-cpp
    voxelfactory
)