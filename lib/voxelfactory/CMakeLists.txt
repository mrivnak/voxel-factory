project(voxelfactory)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES
    src/voxelfactory.cpp
)

add_library(voxelfactory STATIC ${SOURCES})

target_include_directories(voxelfactory PUBLIC
    ./include
)

# Set symbol visibility to hidden for GCC and Clang
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(voxelfactory PRIVATE "-fvisibility=hidden")
    target_compile_options(voxelfactory PRIVATE "-fvisibility-inlines-hidden")
endif()