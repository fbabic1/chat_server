#include "definitions.h"
#include "lib/utils/connection.h"
#include "lib/utils/details.h"

#include <algorithm>
#include <atomic>
#include <iostream>
#include <cstring>
#include <thread>
#include <vector>

#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

void setupServerDetails() {
    utils::server::Details &details = utils::server::Details::instance();
    details.port = SERVER_PORT;
    details.maxConnections = MAX_QUEUED_CONNECTIONS;
}

int getServerSocket() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Error: failed to establish a socket" << std::endl;
        exit(EXIT_FAILURE);
    }
    return serverSocket;
}

sockaddr_in getSocketAddress() {
    sockaddr_in socketAddress{
        .sin_family = AF_INET,
        .sin_port = htons(utils::server::Details::getPort()),
    };
    socketAddress.sin_addr.s_addr = INADDR_ANY;
    return socketAddress;
}

void bindAndListen(int serverSocket, const sockaddr_in& socketAddress) {
    if (bind(serverSocket, (struct sockaddr*)&socketAddress, sizeof(socketAddress)) < 0) {
        std::cerr << "Error: failed to bind to socket" << std::endl;
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, utils::server::Details::getMaxConnections()) < 0) {
        std::cerr << "Error: failed to listen on socket" << std::endl;
        close(serverSocket);
        exit(EXIT_FAILURE);
    }
}

utils::Connection createNewConnection(int serverSocket) {
    sockaddr_in clientAddress;
    socklen_t clientLen = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientLen);
    if (clientSocket < 0) {
        std::cerr << "Error: failed to accept connection" << std::endl;
        exit(EXIT_FAILURE);
    }

    return utils::Connection(clientSocket);
}

void processMessage(const utils::Connection& conn, std::vector<utils::Connection>& connections) {
    utils::Message message = conn.readMessage();
    if (message.getContent().empty()) {
        std::clog << "Connection closed by user " << conn.getUser().getUserName() << std::endl;
        close(conn.getSocket());
        connections.erase(std::remove_if(connections.begin(), connections.end(),
            [&conn](const utils::Connection& c) { return c.getSocket() == conn.getSocket(); }),
            connections.end());
    } else {
        std::clog << "Message: " << message << std::endl;
        for (auto& otherConn : connections) {
            if (otherConn.getSocket() != conn.getSocket()) {
                otherConn.sendMessage(message.toString());
            }
        }
    }
}

void processingLoop(int serverSocket, std::unique_ptr<std::atomic<bool>>& running) {
    fd_set connectionSet;
    std::vector<utils::Connection> connections;

    while (running->load()) {
        FD_ZERO(&connectionSet);
        FD_SET(serverSocket, &connectionSet);
        int maxSocket = serverSocket;

        for (const auto& conn : connections) {
            FD_SET(conn.getSocket(), &connectionSet);
            maxSocket = std::max(maxSocket, conn.getSocket());
        }

        std::clog << "Waiting for activity..." << std::endl;
        int activity = select(maxSocket + 1, &connectionSet, nullptr, nullptr, nullptr);
        if (!running->load()) {
            break;
        }
        if (activity < 0) {
            std::cerr << "Error: select() failed" << std::endl;
            break;
        }

        std::clog << "Activity detected" << std::endl;
        if (FD_ISSET(serverSocket, &connectionSet)) {
            utils::Connection newConnection = createNewConnection(serverSocket);
            std::clog << "New connection accepted: user " << newConnection.getUser().getUserName() << std::endl;
            connections.push_back(std::move(newConnection));
        }

        for (auto& conn : connections) {
            if (FD_ISSET(conn.getSocket(), &connectionSet)) {
                processMessage(conn, connections);
            }
        }
    }
}

void waitForShutdown(int serverSocket) {
    std::string command;
    while (true) {
        std::getline(std::cin, command);
        if (command == SHUTDOWN_COMMAND) {
            std::clog << "Shutting down server..." << std::endl;
            send(serverSocket, command.c_str(), command.size(), 0);
            break;
        }
    }
}

void startProcessingLoop(int serverSocket) {
    auto running = std::make_unique<std::atomic<bool>>(true);

    std::thread([&running, serverSocket]() {
        waitForShutdown(serverSocket);
        running->store(false);
    }).detach();

    processingLoop(serverSocket, running);
}

int main() {
    std::clog << "Server binary started" << std::endl;
    setupServerDetails();

    int serverSocket = getServerSocket();
    sockaddr_in socketAddress = getSocketAddress();
    bindAndListen(serverSocket, socketAddress);

    startProcessingLoop(serverSocket);

    close(serverSocket);
    return EXIT_SUCCESS;
}