#ifndef LIB_CONN_CONNECTION_H
#define LIB_CONN_CONNECTION_H

#include "lib/utils/message.h"
#include "lib/utils/user.h"

#include <string>
#include <thread>

namespace utils {

class Connection {
    int socket;
    User user;

    bool moved;

    static std::string readMessageString(int socket);

public:
    void sendMessage(const std::string &message);
    Message readMessage() const;

    const int getSocket() const;
    const User& getUser() const;

    Connection(int sock);
    ~Connection();

    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;
    Connection(Connection&& other) noexcept;
    Connection& operator=(Connection&& other) noexcept;
};

} // namespace utils

#endif // LIB_CONN_CONNECTION_H