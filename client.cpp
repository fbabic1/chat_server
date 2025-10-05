#include "definitions.h"

#include "lib/config/config.h"
#include "lib/utils/details.h"
#include "lib/utils/user.h"

#include <iostream>
#include <cstring>
#include <thread>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

void setupClientDetails() {
    utils::client::Details &details = utils::client::Details::instance();
    details.address = DEFAULT_SERVER_ADDRESS;
    details.port = SERVER_PORT;
}

utils::User getUserDetails() {
    std::cout << "Enter your username: ";
    std::string input;
    std::getline(std::cin, input);

    return utils::User(input);
}

int createSocket() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Error: failed to create socket" << std::endl;
        exit(EXIT_FAILURE);
    }
    return sock;
}

int setupConnection() {
    int sock = createSocket();
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(utils::client::Details::getPort());
    inet_pton(AF_INET, utils::client::Details::getAddress().c_str(), &serverAddress.sin_addr);

    if (connect(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Error: failed to connect to server" << std::endl;
        exit(EXIT_FAILURE);
    }
    return sock;
}

int main() {
    std::clog << "Client binary started" << std::endl;
    setupClientDetails();

    utils::User user = getUserDetails();

    std::clog << "Connecting to server at " << utils::client::Details::getAddress() << ":" << utils::client::Details::getPort() << " as user " << user.getUserName() << std::endl;
    int serverSocket = setupConnection();
    std::clog << "Connected to server" << std::endl;

    const std::string userData = user.toString();
    std::clog << "Sending user data" << std::endl;
    send(serverSocket, userData.c_str(), userData.size(), 0);

    // listen for messages from server
    std::thread listener([&serverSocket]() {
        char buffer[MAX_MSG_SIZE];
        while (true) {
            memset(buffer, 0, MAX_MSG_SIZE);
            int bytesRead = read(serverSocket, buffer, MAX_MSG_SIZE - 1);
            if (bytesRead <= 0) {
                std::clog << "Disconnected from server" << std::endl;
                close(serverSocket);
                exit(EXIT_SUCCESS);
            }
            buffer[bytesRead] = '\0';
            std::cout << buffer << std::endl;
        }
    });
    listener.detach();

    // read messages from stdin and send to server
    std::string message;
    while (true) {
        std::getline(std::cin, message);
        if (message == QUIT_COMMAND) {
            std::clog << "Quitting..." << std::endl;
            break;
        }
        send(serverSocket, message.c_str(), message.size(), 0);
    }
    return 0;
}