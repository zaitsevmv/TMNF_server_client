cmake_minimum_required(VERSION 3.12)

project(Core)
set(CMAKE_CXX_STANDARD 20)

set(PLUGIN_SOURCE_DIR ${CMAKE_SOURCE_DIR})

include_directories(${PLUGIN_SOURCE_DIR}/BaseStructures)

add_subdirectory(BasePlugin)
add_subdirectory(Tests)

add_executable(main main.cpp)
target_link_directories(main PRIVATE 
    ${PLUGIN_SOURCE_DIR}/BasePlugin)

target_link_libraries(main base_plugin)