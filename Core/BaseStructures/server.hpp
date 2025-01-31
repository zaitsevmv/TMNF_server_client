#include <cstdint>
#include <string>

namespace base_types{
    struct Server{
    public:
        std::string ip;
        uint16_t serverPort;
        uint16_t p2pPort;
        uint16_t rpcPort;
    };
}