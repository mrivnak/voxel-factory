project(gdexample)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES
    src/gdexample.cpp
    src/register_types.cpp
)

add_library(gdexample SHARED ${SOURCES})

target_include_directories(gdexample PUBLIC
    ./include
)

set_target_properties(gdexample PROPERTIES
    OUTPUT_NAME "gdexample.${CMAKE_SYSTEM_NAME}.${CMAKE_BUILD_TYPE}.${CMAKE_SYSTEM_PROCESSOR}"
)

# Set symbol visibility to hidden for GCC and Clang
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(gdexample PRIVATE "-fvisibility=hidden")
    target_compile_options(gdexample PRIVATE "-fvisibility-inlines-hidden")
endif()

target_link_libraries(gdexample PRIVATE
    godot-cpp
    voxelfactory
)