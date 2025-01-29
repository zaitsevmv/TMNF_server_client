#include "../BasePlugin/base_plugin.hpp"
// #include "base_plugin.hpp"

int main(){
    auto server = std::make_shared<base_types::Server>(base_types::Server("127.0.0.0", 2350, 3450));
    std::string name = "base_name";
    auto plugin = BasePlugin(name, server.get());
    plugin.StartClient();
    while(true);
}