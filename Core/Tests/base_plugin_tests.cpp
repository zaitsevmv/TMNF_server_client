#include "gtest/gtest.h"

#include "../BasePlugin/base_plugin.hpp"

TEST(ConnectionTest, BasePluginTests){
    auto svr = base_types::Server("127.0.0.1", 2350, 5000);
    std::string name = "default_name";
    auto plugin = BasePlugin(name, svr);
    auto st = plugin.StartClient();
    EXPECT_EQ(st, BasePlugin::status::down);
}

int main(){
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}