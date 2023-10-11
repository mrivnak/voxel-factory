project(voxelfactory)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES
    src/voxelfactory.cpp
)

add_library(voxelfactory STATIC ${SOURCES})

target_include_directories(voxelfactory PUBLIC
    ./include
)
