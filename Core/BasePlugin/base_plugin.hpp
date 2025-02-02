#include <fstream>
#include <optional>
#include <string>
#include <memory>
#include <mutex>
#include <thread>

// #include "request.hpp"
#include "server.hpp"
#include "base_types.hpp"

class BasePlugin{
public:
    enum class status{
        ready, down
    };

    enum class messageError{
        ok, error
    };

    BasePlugin(const std::string& pluginId, const base_types::Server& svr);
    ~BasePlugin();

    void run(); //should be remade for child plugins

    status StartClient();

    void StopClient();

    status listen(); //should be fixed. Can make separate listen for callbacks and requests. This can be 2 base plugins.

    void AddMessage(const std::string& message);

    std::string getPluginId();

    BasePlugin(const BasePlugin& pl) = delete;
    BasePlugin operator=(const BasePlugin&) = delete;
private:
    int clientSocket;

    base_types::Server server;
    std::string pluginId;

    uint32_t currentHandler;

    base_types::xmlResponseQueue pendingMessages;

    std::thread listeningThread;

    status status_ = status::down;

    std::ofstream pluginLogs;

    void WriteToLogs(const std::string& message, const bool isError);
    void WriteToLogs(const std::string& message){
        WriteToLogs(message, false);
    }

    // messageError SendRequest(const base_types::Request& request);
    messageError SendXML(const std::string& xml);
};