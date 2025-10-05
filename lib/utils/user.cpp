#include "lib/utils/user.h"

namespace utils::detail {
    
std::string removeNewline(const std::string& str) {
    if (!str.empty() && str.back() == '\n') {
        return str.substr(0, str.size() - 1);
    }
    return str;
}

} // namespace utils::detail
