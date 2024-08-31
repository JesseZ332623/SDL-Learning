#include "./defs.hpp"

#include "SDL.h"
#include "SDL_net.h"

#include <cstring>

TCPsocket clientSocket;     // 客户端套接字

/**
 * @brief 解析一个由整数表示的 IPv4 地址，存储在一个字符串中返回。
*/
static std::string soloveIPV4Address(const Uint32 __IPAddress);

static std::string soloveIPV4Address(const Uint32 __IPAddress)
{
    std::string IPv4String;
    unsigned char * hostPointer = (unsigned char *)(&__IPAddress);

    for (size_t size = 0; size < sizeof(Uint32); ++size)
    {
        IPv4String += std::to_string(*hostPointer);
        IPv4String.push_back('.');
        ++hostPointer;
    }

    IPv4String.pop_back();

    return IPv4String;
}

/**
 * @brief 输出主机号与端口号。
 * 
 * @param __IPaddress  IPv4 hostname resolution
*/
void showIPaddress(const IPaddress * __IPaddress);

void showIPaddress(const IPaddress * __IPaddress)
{
    if (__IPaddress != nullptr)
    {
        NOTIFY_LOG(
            "Host: [" + soloveIPV4Address(__IPaddress->host) + ']' +
            " Port: [" + std::to_string(__IPaddress->port) + "]\n"
        );
    }
}

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

    CORRECT_LOG('[' + getCurrentTime() + "] Client: Open TCP Connect Success!\n");
    showIPaddress(&ipaddress);

    while (true)
    {
        char buffer[MESSAGE_BUFFER_SIZE] = {0};

        NOTIFY_LOG("Enter message: \n");
        std::fgets(buffer, MESSAGE_BUFFER_SIZE, stdin);
        *std::strchr(buffer, '\n') = '\0';

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
    system("cls");

    printSplitLine(30, '-');
    NOTIFY_LOG("Client: Close TCP Socket.\n");
    SDLNet_TCP_Close(clientSocket);

    NOTIFY_LOG("Client: SDL Net Quit.\n");
    SDLNet_Quit();

    CORRECT_LOG("Client Exit.\n");
    printSplitLine(30, '-');
    exit(EXIT_SUCCESS);
}