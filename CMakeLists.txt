cmake_minimum_required(VERSION 3.15)

project(wu-syswin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(X11 REQUIRED)

add_library(
    ${PROJECT_NAME} INTERFACE
)

target_include_directories(
    ${PROJECT_NAME} INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${X11_INCLUDE_DIR}
)

target_link_libraries(
    ${PROJECT_NAME} INTERFACE
        ${X11_LIBRARIES}
)

add_subdirectory(demos)