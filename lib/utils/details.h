#ifndef LIB_UTILS_SERVER_DETAILS_H
#define LIB_UTILS_SERVER_DETAILS_H

#include <exception>
#include <string>

namespace utils {

namespace server {

struct Details {
    int port;
    int maxConnections;

    static Details& instance() {
        static Details instance;
        return instance;
    }

    static int getPort() {
        return instance().port;
    }

    static int getMaxConnections() {
        return instance().maxConnections;
    }

private:
    Details() = default;
};

} // namespace server

namespace client {

struct Details {
    std::string address;
    int port;

    static Details& instance() {
        static Details instance;
        return instance;
    }

    static const std::string& getAddress() {
        return instance().address;
    }

    static int getPort() {
        return instance().port;
    }

private:
    Details() = default;
};

} // namespace client

} // namespace utils

#endif // LIB_UTILS_SERVER_DETAILS_H