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

    BasePlugin(const std::string& pluginId, base_types::Server* const svr);
    ~BasePlugin();

    status StartClient();

    status StopClient();

    // messageError SendRequest(const base_types::Request& request);

    void AddMessage(const std::string& message);

    // virtual messageError HandleMessages();

    std::string getPluginId();

    BasePlugin(const BasePlugin& pl) = delete;
    BasePlugin operator=(const BasePlugin&) = delete;
private:
    int clientSocket;

    std::shared_ptr<base_types::Server> server;
    std::string pluginId;

    base_types::xmlResponseQueue pendingMessages;

    status _status = status::down;
};