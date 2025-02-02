#include <variant>
#include <string>
#include <vector>
#include <queue>

namespace base_types{
    using xmlParam = std::variant<int, std::string, bool, double>;

    using stringVec = std::vector<std::string>;

    using xmlResponseQueue = std::queue<std::string>;
}