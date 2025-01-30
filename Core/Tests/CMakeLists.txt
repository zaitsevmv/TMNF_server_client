include(FetchContent)

# Fetch GoogleTest
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2
)
FetchContent_MakeAvailable(googletest)

add_executable(base_plugin_tests base_plugin_tests.cpp)

target_link_directories(base_plugin_tests PRIVATE 
    ${PLUGIN_SOURCE_DIR}/BasePlugin
    ${gtest_SOURCE_DIR}/include)

target_link_libraries(base_plugin_tests base_plugin gtest_main)