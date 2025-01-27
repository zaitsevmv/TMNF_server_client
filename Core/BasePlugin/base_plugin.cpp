#include "base_plugin.hpp"

const bool BasePlugin::Server::operator==(const Server& rhs){
    return this->host == rhs.host && this->serverPort == rhs.serverPort && this->p2pPort == rhs.p2pPort;
};

void BasePlugin::SetServer(Server* const svr)
{
    server.reset(svr);
}