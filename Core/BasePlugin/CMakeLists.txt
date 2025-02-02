add_library(base_plugin STATIC 
    base_plugin.hpp base_plugin.cpp)

target_include_directories(base_plugin PRIVATE ${PLUGIN_SOURCE_DIR}/BaseStructures)