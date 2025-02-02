add_library(callback_listener STATIC 
    callback_listener.hpp 
    callback_listener.cpp)
    
target_link_libraries(callback_listener 
    base_plugin)

target_include_directories(callback_listener PRIVATE 
    ${PLUGIN_SOURCE_DIR}/BasePlugin)