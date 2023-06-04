cmake_minimum_required(VERSION 3.0)
project(dsrs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    main.cpp
    initialization.cpp
    initialTesting.cpp
    common.cpp
    util.cpp
)

add_executable(dsrs ${SOURCES})

# Set include directories
target_include_directories(dsrs PRIVATE "include")
# Thanks ChatGPT, very cool