#include <optional>
#include <variant>
#include <string>
#include <concepts>

#include "base_types.hpp"

class Request
{
public:
    Request(std::string method, std::initializer_list<base_types::requestParam> params)
        :method{method}, params{params} {
        GetXMLRequest();
    };

    Request(std::string method, std::vector<base_types::requestParam> params)
        :method{method}, params{params} {
        GetXMLRequest();
    };

    template<std::convertible_to<base_types::requestParam>... Args>
    Request(std::string method, Args... args)
        :method{method}, params{args} {
        GetXMLRequest();
    };

    std::string GetXMLRequest(){
        if(!xmlRequest.empty()){
            return xmlRequest;
        }

    }
private:
    std::string method;
    std::vector<base_types::requestParam> params;
    std::string xmlRequest;
};
