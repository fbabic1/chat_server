#ifndef LIB_UTILS_MESSAGE_H
#define LIB_UTILS_MESSAGE_H

#include "lib/utils/user.h"

#include <string>
#include <ostream>

namespace utils {

class Message {
    User sender;
    std::string content;

public:
    friend std::ostream& operator <<(std::ostream& os, const Message& msg);

    Message(const User &user, const std::string &msg) : sender(user), content(msg) {}
    ~Message() = default;

    const std::string &getContent() const {return content;}
    const User& getSender() const {return sender;}

    const std::string toString() const {
        return "[" + sender.getUserName() + "]: " + content;
    }
};

} // namespace utils

#endif // LIB_UTILS_MESSAGE_H