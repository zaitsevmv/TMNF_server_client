#include <optional>
#include <string>
#include <memory>


class BasePlugin{
public:
    struct Server{
    public:
        std::string host;
        u_short serverPort;
        u_short p2pPort;

        const bool operator==(const Server&){};
    };

    static BasePlugin& Instance(){
        static BasePlugin theSingleInstance;
        return theSingleInstance;
    }

    void SetServer(Server* const svr);

    BasePlugin(const BasePlugin& pl) = delete;
    BasePlugin operator=(const BasePlugin&) = delete;
private:
    std::shared_ptr<Server> server;
    BasePlugin(){};

    
};