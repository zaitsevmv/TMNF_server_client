#include <cstdint>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <sys/types.h>
#include <thread>
#include <vector>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "base_plugin.hpp"

constexpr int BUFFER_SIZE = 600;
constexpr int SIZE_SIZE = 4;
constexpr int HANDLER_SIZE = 4;

BasePlugin::BasePlugin(const std::string& plugin, const base_types::Server& svr)
    : pluginId{plugin}, server{svr} {
    pluginLogs.open("plugin_logs.txt", std::ios::out | std::ios::app);
    pluginLogs << ". . ." << std::endl;
}

BasePlugin::~BasePlugin() {
    StopClient();
}

void BasePlugin::run(){
    StartClient();
    std::string initialMessage = "<?xml version=\"1.0\"?><methodCall><methodName>Authenticate</methodName><params><param><value>SuperAdmin</value></param><param><value>yr4K5AxSGqkb6tEP928HRg</value></param></params></methodCall>";
    if(SendXML(initialMessage) != messageError::ok){
        WriteToLogs("Can't send auth message.", 1);
        return;
    }
    listen();
    if(status_ != status::ready){
        return;
    }
    while(pendingMessages.empty()); //wait for responce
    pendingMessages.pop();
    std::string testMessage = "<?xml version=\"1.0\"?><methodCall><methodName>SetServerName</methodName><params><param><value>SmallBalls</value></param></params></methodCall>";
    if(SendXML(testMessage) != messageError::ok){
        WriteToLogs("Can't send test message.", 1);
        return;
    }
    while(pendingMessages.empty()); //wait for responce
}

BasePlugin::status BasePlugin::StartClient() {
    if(status_ == status::ready) return status_;
    if(status_ != status::down) StopClient();
    WriteToLogs("Connecting to server.");
    status_ = status::down;
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket == -1) {
        WriteToLogs("Socket creation failed. Error # " + std::to_string(errno), 1);
        return status_;
    }

    uint32_t socketIp;
    if(inet_pton(AF_INET, server.ip.c_str(), &socketIp) < 0) {
        WriteToLogs("Ip convention failed. Error # " + std::to_string(errno), 1);
        return status_;
    }

    sockaddr_in serverInfo;
    serverInfo.sin_family = AF_INET;
    serverInfo.sin_port = htons(server.rpcPort);
    serverInfo.sin_addr = in_addr(socketIp);

    if(connect(clientSocket, (sockaddr*)&serverInfo, sizeof(serverInfo)) != 0){
        WriteToLogs("Connecting to server failed. Error # " + std::to_string(errno), 1);
        return status_;
    }

    WriteToLogs("Getting header.");

    std::vector<char> sizeBytes(SIZE_SIZE);
    auto sizeResponseSize = recv(clientSocket, sizeBytes.data(), SIZE_SIZE, 0);
    if(sizeResponseSize != SIZE_SIZE){
        WriteToLogs("Can't get initial response. Error # " + std::to_string(errno), 1);
        return status_;
    }

    uint32_t sizeValue = 0;
    for(auto i = 0; i < SIZE_SIZE; i++){
        sizeValue |= ((static_cast<uint32_t>(sizeBytes[i]) & 0xFF) << 8*i);
    }
    if(sizeValue > BUFFER_SIZE){
        WriteToLogs("Got a very big header.", 1);
        return status_;
    }

    std::vector<char> headerBytes(sizeValue);
    auto headerResponseSize = recv(clientSocket, headerBytes.data(), headerBytes.size(), 0);
    if(headerResponseSize != headerBytes.size()){
        WriteToLogs("Can't get header. Error # " + std::to_string(errno), 1);
        return status_;
    }
    if(std::string(headerBytes.data(), headerBytes.size()) != "GBXRemote 2"){
        WriteToLogs("Got incorrect header from server. Got: " + std::string(headerBytes.data()), 1);
        return status_;
    }
    status_ = status::ready;
    WriteToLogs("Client connected to server.");
    return status_;
}

void BasePlugin::StopClient() {
    close(clientSocket);
    status_ = status::down;
    WriteToLogs("Client socket closed.");
}

BasePlugin::messageError BasePlugin::SendXML(const std::string& xml){
    if(status_ != status::ready) return messageError::error;
    std::vector<char> sizeBytes(SIZE_SIZE), handlerBytes(HANDLER_SIZE), dataBytes(xml.cbegin(), xml.cend());
    for(auto i = 0; i < HANDLER_SIZE; i++){
        handlerBytes[i] = (currentHandler >> 8*i) & UINT8_MAX; 
    }
    if(xml.size() > (BUFFER_SIZE - SIZE_SIZE - HANDLER_SIZE)){
        WriteToLogs("Message too long.", 1);
        return messageError::error;
    }
    for(auto i = 0; i < SIZE_SIZE; i++){
        sizeBytes[i] = (xml.size() >> 8*i) & UINT8_MAX; 
    }
    std::vector<char> message;
    message.reserve(BUFFER_SIZE);
    message.insert(message.cend(), sizeBytes.cbegin(), sizeBytes.cend());
    message.insert(message.cend(), handlerBytes.cbegin(), handlerBytes.cend());
    message.insert(message.cend(), dataBytes.cbegin(), dataBytes.cend());

    auto requestSize = send(clientSocket, message.data(), message.size(), 0);
    if(requestSize != message.size()){
        WriteToLogs("Message not sent fully. Expected: " 
            + std::to_string(message.size()) 
            + ". Got: " 
            +  std::to_string(requestSize) + ".", 1);
        return messageError::error;
    }
    WriteToLogs("Sent message to server: " + xml);
    ++currentHandler;
    return messageError::ok;
}

BasePlugin::status BasePlugin::listen()
{
    if(status_ != status::ready) return status_;
    auto listeningProcess = [&](){
        std::vector<char> messageSize(SIZE_SIZE), messageHandler(HANDLER_SIZE);
        while(status_ != status::down){
            auto sizeSize = recv(clientSocket, messageSize.data(), messageSize.size(), 0);
            if(sizeSize != SIZE_SIZE){
                WriteToLogs("Can't get message from Server. Error # " + std::to_string(errno), 1);
                StopClient();
                return status_;
            } else if (sizeSize == 0) { 
                WriteToLogs("Server closed connection");
                StopClient();
                return status_;
            }

            uint32_t sizeValue = 0;
            for(auto i = 0; i < SIZE_SIZE; i++){
                sizeValue |= ((static_cast<uint32_t>(messageSize[i]) & 0xFF) << 8*i);
            }
            if(sizeValue > BUFFER_SIZE){
                WriteToLogs("Got a very big message from server.", 1);
                StopClient();
                return status_;
            }
            
            auto handlerSize = recv(clientSocket, messageHandler.data(), messageHandler.size(), 0);

            std::vector<char> data(sizeValue);
            auto dataSize = recv(clientSocket, data.data(), data.size(), 0);
            if(dataSize != sizeValue){
                WriteToLogs("Got smaller message than expected.", 1);
                StopClient();
                return status_;
            }

            WriteToLogs("Got message from server: " + std::string(data.data(), data.size()));
            AddMessage(std::string(data.data()));
        }
        return status_;
    };
    listeningThread = std::thread(listeningProcess);
    listeningThread.detach();
    return status_;
}

void BasePlugin::WriteToLogs(const std::string& message, const bool isError){
    auto t = std::time(nullptr);
    auto currentTime = *std::localtime(&t);
    pluginLogs << std::put_time(&currentTime, "%d/%m/%y %H:%M:%S");
    if(isError) pluginLogs << " Error:";
    pluginLogs << " [" << pluginId << "] " << message << std::endl;
}

void BasePlugin::AddMessage(const std::string& message){
    pendingMessages.push(message);
}

// messageError BasePlugin::SendRequest(const base_types::Request& request) {
//     if(_status != status::ready) return messageError::error;
// }

// void BasePlugin::AddMessage(const std::string& message) {
//     pendingMessages.push(message);
// }
