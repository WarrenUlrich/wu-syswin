cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    simple-window
        src/simple-window.cpp
)

target_link_libraries(
    simple-window PRIVATE
        wu-syswin
)