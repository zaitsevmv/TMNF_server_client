#include <iostream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "base_plugin.hpp"

BasePlugin::BasePlugin(const std::string& plugin, base_types::Server* const svr)
    : pluginId{plugin} {
    server.reset(svr);
}

BasePlugin::~BasePlugin() {
    StopClient();
}

BasePlugin::status BasePlugin::StartClient() {
    if(_status == status::ready || _status == status::writing) return _status;
    if(_status != status::down) StopClient();
    //lets hope server stops
    _status = status::down;
    clientSocket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if(clientSocket == -1) return _status;

    int socketIp;
    if(inet_pton(AF_INET, server.get()->ip, &socketIp) < 0) return _status;

    sockaddr_in serverInfo;
    serverInfo.sin_family = AF_INET;
    serverInfo.sin_port = htons(server.get()->p2pPort);
    serverInfo.sin_addr = in_addr(socketIp);

    if(connect(clientSocket, (sockaddr*)&serverInfo, sizeof(serverInfo)) == 0){
        _status = status::ready;
    }
    std::cout << "Client created" << std::endl;
    return _status;
}

BasePlugin::status BasePlugin::StopClient() {
    if(_status == status::down) return _status;
    close(clientSocket);
    _status = status::down;
    return _status;
}

// messageError BasePlugin::SendRequest(const base_types::Request& request) {
//     if(_status != status::ready) return messageError::error;
// }

// void BasePlugin::AddMessage(const std::string& message) {
//     pendingMessages.push(message);
// }
