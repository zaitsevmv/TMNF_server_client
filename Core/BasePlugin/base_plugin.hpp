#include <optional>

class BasePlugin{
public:
    static BasePlugin& Instance(){
        static BasePlugin theSingleInstance;
        return theSingleInstance;
    }
private:
    BasePlugin(){};
    BasePlugin(const BasePlugin& pl) = delete;
    BasePlugin operator=(const BasePlugin&) = delete;
};