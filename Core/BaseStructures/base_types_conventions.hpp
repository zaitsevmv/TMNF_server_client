#include <optional>

#include "base_types.hpp"

namespace base_types{
    std::optional<std::string> RequestParam2xml(const base_types::xmlParam& p){
        return std::visit([](auto arg)->std::optional<std::string>{
            using T = std::decay<decltype(arg)>;
            if(std::is_same<T, std::string>){
                return arg;
            }
            if(std::is_same<T, int>){
                return std::to_string(arg);
            }
            if(std::is_same<T, bool>){
                return std::to_string(arg);
            }
            if(std::is_same<T, double>){
                return std::to_string(arg); 
            }
            return std::nullopt;
        }, p);
    }
}