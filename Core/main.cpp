#include <string>

#include "BasePlugin/base_plugin.hpp"

int main(){
    auto server = base_types::Server("127.0.0.1", 2350, 3250, 5000);
    std::string name = "base_name";
    auto plugin = BasePlugin(name, server);
    plugin.StartClient();
    std::string initialMessage = "<?xml version=\"1.0\"?><methodCall><methodName>Authenticate</methodName><params><param><value>SuperAdmin</value></param><param><value>yr4K5AxSGqkb6tEP928HRg</value></param></params></methodCall>";
    plugin.listen();
}