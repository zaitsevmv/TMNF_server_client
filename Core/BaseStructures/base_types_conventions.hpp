#include <optional>

#include "base_types.hpp"

namespace base_types{
    // std::optional<stringVec> RequestParam2xml(const base_types::requestParam& p){
    //     return std::visit([](auto arg)->std::optional<stringVec>{
    //         using T = std::decay<decltype(arg)>;
    //         if(std::is_same<T, std::string>){
    //             return stringVec{arg};
    //         }
    //         if(std::is_same<T, int>){
    //             return stringVec{std::to_string(arg)};
    //         }
    //         if(std::is_same<T, std::vector<std::string>>){
    //             return arg;
    //         }
    //         if(std::is_same<T, std::vector<int>>){
    //             stringVec rtnVec;
    //             std::transform(arg.cbegin(), arg.cend(), 
    //                 std::back_inserter(rtnVec), [](auto num){return std::to_string(num);});
    //             return rtnVec; 
    //         }
    //         return std::nullopt;
    //     }, p);
    // }
}