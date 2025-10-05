#ifndef LIB_UTILS_USER_H
#define LIB_UTILS_USER_H

#include <string>

namespace utils {

namespace detail {

std::string removeNewline(const std::string& str);

} // namespace detail

class UserImpl {
    std::string userName;

public:
    UserImpl(const std::string& username) : userName(detail::removeNewline(username)) {}
    ~UserImpl() = default;

    std::string getUserName() const {
        return userName;
    }

    std::string toString() const {
        return userName;
    }
};

using User = UserImpl; // Alias for easier usage

} // namespace utils

#endif // LIB_UTILS_USER_H