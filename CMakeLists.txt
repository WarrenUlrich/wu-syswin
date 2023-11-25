cmake_minimum_required(VERSION 3.15)

project(wu-syswin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(X11 REQUIRED)

option(WU_SYSWIN_ENABLE_VULKAN "Enable Vulkan support in wu-syswin" ON)

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

if(WU_SYSWIN_ENABLE_VULKAN)
    find_package(Vulkan REQUIRED)
    target_link_libraries(${PROJECT_NAME} INTERFACE Vulkan::Vulkan)
    target_compile_definitions(${PROJECT_NAME} INTERFACE WU_SYSWIN_VULKAN_SUPPORT)
endif()


add_subdirectory(demos)