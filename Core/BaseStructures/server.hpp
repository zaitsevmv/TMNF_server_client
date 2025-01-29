#include <cstdint>

namespace base_types{
    struct Server{
    public:
        char* ip;
        uint16_t serverPort;
        uint16_t p2pPort;
    };
}