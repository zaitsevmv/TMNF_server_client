add_library(base_request_plugin STATIC 
    base_request_plugin.hpp 
    base_request_plugin.cpp)
    
target_link_libraries(base_request_plugin 
    base_plugin)

target_include_directories(base_request_plugin PRIVATE 
    ${PLUGIN_SOURCE_DIR}/BasePlugin)