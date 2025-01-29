#include <optional>
#include <variant>
#include <string>
#include <concepts>

#include "base_types.hpp"
#include "base_types_conventions.hpp"

namespace base_types{
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
            xmlRequest = "<?xml version=\"1.0\" encoding=\"utf-8\" ?>";
            xmlRequest += "<methodCall><methodName> " + method + " </methodName>";
            std::vector<std::string> xmlParamVector;
            xmlParamVector.reserve(params.size());
            for(const auto& parameter: params){
                auto stringParam = base_types::RequestParam2xml(parameter);
                if(stringParam == std::nullopt){
                    continue;
                }
                xmlParamVector.push_back(std::move(stringParam.value().front()));
            } 
            if(!xmlParamVector.empty()){
                xmlRequest += "<params>";
                for(const auto& p: xmlParamVector){
                    xmlRequest += "<param><value>";
                    xmlRequest += p;
                    xmlRequest += "</param></value>";
                }
                xmlRequest += "</params>";
            }
            xmlRequest += "</methodCall>";
        }
    private:
        std::string method;
        std::vector<base_types::requestParam> params;
        std::string xmlRequest;
    };
}