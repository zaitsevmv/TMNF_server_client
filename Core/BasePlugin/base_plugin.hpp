#include <fstream>
#include <optional>
#include <string>
#include <memory>
#include <mutex>

// #include "request.hpp"
#include "server.hpp"
#include "base_types.hpp"

class BasePlugin{
public:
    enum class status{
        ready, writing, writingError, down
    };

    enum class messageError{
        ok, error
    };

    BasePlugin(const std::string& pluginId, const base_types::Server& svr);
    ~BasePlugin();

    status StartClient();

    void StopClient();

    status listen();

    void AddMessage(const std::string& message);

    // messageError HandleMessages();

    std::string getPluginId();

    BasePlugin(const BasePlugin& pl) = delete;
    BasePlugin operator=(const BasePlugin&) = delete;
private:
    int clientSocket;

    base_types::Server server;
    std::string pluginId;

    uint32_t currentHandler;

    base_types::xmlResponseQueue pendingMessages;

    status status_ = status::down;

    std::ofstream pluginLogs;

    void WriteToLogs(const std::string& message, const bool isError);
    void WriteToLogs(const std::string& message){
        WriteToLogs(message, false);
    }

    // messageError SendRequest(const base_types::Request& request);
    messageError SendXML(const std::string& xml);
};