#include <string>

#include "base_plugin.hpp"

int main(){
    auto server = base_types::Server("127.0.0.1", 2350, 3250, 5000);
    std::string name = "base_name";
    auto plugin = BasePlugin(name, server);
    plugin.run();
}