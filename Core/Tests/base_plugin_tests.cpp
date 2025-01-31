#include <thread>

#include "gtest/gtest.h"

#include "../BasePlugin/base_plugin.hpp"

TEST(FailConnectionTest, BasePluginTests){
    auto svr = base_types::Server("127.0.0.1", 2350, 5001);
    std::string name = "default_name";
    auto plugin = BasePlugin(name, svr);
    EXPECT_EQ(plugin.StartClient(), BasePlugin::status::down);
}

TEST(SuccessConnectionTest, BasePluginTests){
    auto svr = base_types::Server("127.0.0.1", 2350, 5000);
    std::string name = "default_name";
    auto plugin = BasePlugin(name, svr);
    std::thread ncServerThread(std::system, "nc -l 5000");
    ncServerThread.join();
    EXPECT_EQ(plugin.StartClient(), BasePlugin::status::ready);
}

int main(){
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}