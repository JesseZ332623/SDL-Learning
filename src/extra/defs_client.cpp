#include "./defs.hpp"

#include "SDL.h"
#include "SDL_net.h"

#include <cstring>

void showIPaddress(const IPaddress * __IPaddress);

void showIPaddress(const IPaddress * __IPaddress)
{
    if (__IPaddress != nullptr)
    {
        NOTIFY_LOG(
            "Host: [" + std::to_string(__IPaddress->host) + ']' +
            " Port: [" + std::to_string(__IPaddress->port) + "]\n"
        );
    }
}

TCPsocket clientSocket;

void ClientOperator(const char * __ip, int __port)
{
    /**
     * @brief 初始化 SDL NET 模块
    */
    if (SDLNet_Init() < 0)
    {
        ERROR_LOG(
            "Failed to init SDL Net Module, SDL ERROR: " + std::string(SDLNet_GetError()) + '\n'
        );
        exit(EXIT_FAILURE);
    }

    IPaddress ipaddress;
    if (SDLNet_ResolveHost(&ipaddress, __ip, __port) == -1)
    {
        ERROR_LOG(
            "Failed to resolve host, SDL ERROR: " + std::string(SDLNet_GetError()) + '\n'
        );

        exit(EXIT_FAILURE);
    }


    clientSocket = SDLNet_TCP_Open(&ipaddress);

    if (!clientSocket)
    {
        ERROR_LOG(
            "Failed to open client socket, SDL ERROR: " + std::string(SDLNet_GetError()) + '\n'
        );
        showIPaddress(&ipaddress);

        exit(EXIT_FAILURE);
    }

    while (true)
    {
        char buffer[MESSAGE_BUFFER_SIZE] = {0};

        NOTIFY_LOG("Enter message: \n");
        std::fgets(buffer, MESSAGE_BUFFER_SIZE, stdin);

        SDLNet_TCP_Send(clientSocket, buffer, MESSAGE_BUFFER_SIZE);

        std::memset(buffer, 0, MESSAGE_BUFFER_SIZE);

        int received = SDLNet_TCP_Recv(clientSocket, buffer, MESSAGE_BUFFER_SIZE);

        if (received > 0) {
            CORRECT_LOG("Server response: " + std::string(buffer) + '\n');
        }
        else {
            ERROR_LOG("Error receiving data from server.\n");
        }
    }
}

void clientExitHandle(int)
{
    NOTIFY_LOG("Client: Close TCP Socket.\n");
    SDLNet_TCP_Close(clientSocket);

    NOTIFY_LOG("Client: SDL Net Quit.\n");
    SDLNet_Quit();

    CORRECT_LOG("Client Exit.\n");
    exit(EXIT_SUCCESS);
}