#include <iostream>
#include <vector>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "base_plugin.hpp"

constexpr int BUFFER_SIZE = 600;

BasePlugin::BasePlugin(const std::string& plugin, const base_types::Server& svr)
    : pluginId{plugin}, server{svr} {}

BasePlugin::~BasePlugin() {
    StopClient();
}

BasePlugin::status BasePlugin::StartClient() {
    if(_status == status::ready || _status == status::writing) return _status;
    if(_status != status::down) StopClient();
    _status = status::down;
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket == -1) {
        std::cerr << "Socket creation failed. Error # " << errno << std::endl;
        return _status;
    }

    uint32_t socketIp;
    if(inet_pton(AF_INET, server.ip, &socketIp) < 0) {
        std::cerr << "Ip convention failed. Error # " << errno << std::endl;
        return _status;
    }

    sockaddr_in serverInfo;
    serverInfo.sin_family = AF_INET;
    serverInfo.sin_port = htons(server.p2pPort);
    serverInfo.sin_addr = in_addr(socketIp);

    if(connect(clientSocket, (sockaddr*)&serverInfo, sizeof(serverInfo)) == 0){
        _status = status::ready;
        std::cout << "Client created" << std::endl;
        return _status;
    }
    std::cerr << "Connecting to server failed. Error # " << errno << std::endl;
    return _status;
}

void BasePlugin::StopClient() {
    close(clientSocket);
    _status = status::down;
}

BasePlugin::status BasePlugin::listen()
{
    if(_status != status::ready) return _status;
    std::vector<char> buffer(BUFFER_SIZE), closeMessage(3, 'x'), text(BUFFER_SIZE);
    while(true){
        fgets(text.data(), text.size(), stdin);
        auto packet_size = send(clientSocket, text.data(), strlen(text.data()), 0);
		if (packet_size == -1) {
			std::cout << "Can't send message to Server. Error # " << errno << std::endl;
            StopClient();
            return _status;
		}

        packet_size = recv(clientSocket, buffer.data(), buffer.size()-1, 0);
        if(packet_size == -1){
            std::cout << "Can't get message from Server. Error # " << errno << std::endl;
            StopClient();
            return _status;
        } else if (packet_size == 0) { 
            std::cout << "Server closed connection." << std::endl;
            StopClient();
            return _status;
        }
        buffer[packet_size] = '\0';
        std::cout << "Server's message: " << buffer.data() << std::endl;
        if(strncmp(buffer.data(), closeMessage.data(), closeMessage.size()) == 0){
            StopClient();
            return _status;
        }
    }
    return _status;
}

// messageError BasePlugin::SendRequest(const base_types::Request& request) {
//     if(_status != status::ready) return messageError::error;
// }

// void BasePlugin::AddMessage(const std::string& message) {
//     pendingMessages.push(message);
// }
