add_executable(base_plugin_tests base_plugin_tests.cpp)

target_link_directories(base_plugin_tests PRIVATE 
    ${PLUGIN_SOURCE_DIR}/BasePlugin)

target_link_libraries(base_plugin_tests base_plugin)