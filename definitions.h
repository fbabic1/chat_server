#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

const unsigned int MAX_QUEUED_CONNECTIONS = 5;
const unsigned int SERVER_PORT = 8080;
const unsigned int MAX_MSG_SIZE = 1024;

const std::string DEFAULT_SERVER_ADDRESS = "127.0.0.1";

const std::string SHUTDOWN_COMMAND = "/shutdown";
const std::string QUIT_COMMAND = "/quit";

const unsigned int ADDR_LEN = sizeof(sockaddr_in);

#endif // DEFINITIONS_H