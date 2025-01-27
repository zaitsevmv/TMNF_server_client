#include <variant>
#include <string>
#include <vector>

namespace base_types{
    using requestParam = std::variant<int, std::string, std::vector<int>, std::vector<std::string>>;

    using stringVec = std::vector<std::string>;
}