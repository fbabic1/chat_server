#include "definitions.h"
#include "lib/utils/connection.h"
#include "lib/utils/message.h"

#include <iostream>
#include <thread>
#include <cstring>

#include <unistd.h>

namespace utils {

Connection::Connection(int sock) : socket(sock), user(readMessageString(sock)) {
    std::clog << "Connection " << socket << " established for user " << user.getUserName() << std::endl;
}

Connection::~Connection() {
    if (moved) return;
    close(socket);
    std::clog << "Connection " << socket << " closed" << std::endl;
}

Connection::Connection(Connection&& other) noexcept : socket(other.socket), user(std::move(other.user)), moved(false) {
    other.moved = true;
}

Connection& Connection::operator=(Connection&& other) noexcept {
    if (this != &other) {
        socket = other.socket;
        user = std::move(other.user);
        moved = false;
        other.moved = true;
    }
    return *this;
}

std::string Connection::readMessageString(int socket) {
    char buffer[MAX_MSG_SIZE];
    memset(buffer, 0, MAX_MSG_SIZE);
    read(socket, buffer, MAX_MSG_SIZE);
    buffer[MAX_MSG_SIZE - 1] = '\0';

    const int len = strlen(buffer);
    if (buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    return std::string(buffer);
}

Message Connection::readMessage() const {
    std::string msgStr = readMessageString(socket);
    return Message(user, std::move(msgStr));
}

const int Connection::getSocket() const {
    return socket;
}

const User& Connection::getUser() const {
    return user;
}

void Connection::sendMessage(const std::string &message) {
    send(socket, message.c_str(), message.size(), 0);
}

} // namespace utils