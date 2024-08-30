#include "./defs.hpp"

#include "SDL.h"
#include "SDL_net.h"

#include <cstring>

/**
 * @brief 输出主机号与端口号。
 * 
 * @param __IPaddress  IPv4 hostname resolution
*/
void showIPaddress(const IPaddress * __IPaddress)
{
    if (__IPaddress != nullptr)
    {
        NOTIFY_LOG(
            //"Host: ["  + std::to_string(__IPaddress->host) + ']' +
            "Port: [" + std::to_string(__IPaddress->port) + "]\n"
        );
    }
}

SDLNet_SocketSet serverSocketSet;
TCPsocket        serverSocket;

void ServerOperator(void)
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

    /**
     * @brief 解析 域名，端口号，填充至 ipaddress 结构体中。
    */
    if (SDLNet_ResolveHost(&ipaddress, NULL, PORT) == -1)
    {
        ERROR_LOG(
            "Failed to resolve host, SDL ERROR: " + std::string(SDLNet_GetError()) + '\n'
        );

        exit(EXIT_FAILURE);
    }

    /**
     * @brief 根据 ipaddress 提供的 主机 IP 地址，端口号，
     *        打开 TCP 连接，返回一个 scoket 给 serverSocket。
    */
    serverSocket = SDLNet_TCP_Open(&ipaddress);

    if (!serverSocket)
    {
        ERROR_LOG(
            "Failed to open tcp connect, SDL ERROR: " + std::string(SDLNet_GetError()) + '\n'
        );
        showIPaddress(&ipaddress);

        exit(EXIT_FAILURE);
    }

    CORRECT_LOG('[' + getCurrentTime() + "] Server: Open TCP Connect Success!\n");
    showIPaddress(&ipaddress);

    /**
     * @brief 使用 SDLNet_AllocSocketSet() 来创建套接字集合，从而管理套接字。
    */
    serverSocketSet = SDLNet_AllocSocketSet(2);
    SDLNet_TCP_AddSocket(serverSocketSet, serverSocket);

    while (true)
    {
        TCPsocket clientSocket = SDLNet_TCP_Accept(serverSocket);

        if (clientSocket == nullptr)
        {
#if false
            ERROR_LOG(
                "Couldn't accept client connection, SDL ERROR: " + 
                std::string(SDLNet_GetError()) + '\n'
            );
#endif

            continue;
        }

        SDLNet_TCP_AddSocket(serverSocketSet, clientSocket);

        char buffer[MESSAGE_BUFFER_SIZE] = {0};

        while (true)
        {
            if (SDLNet_CheckSockets(serverSocketSet, TIMEOUT) == -1)
            {
                ERROR_LOG(
                    "Socket check failed, SDL ERROR: " + 
                    std::string(SDLNet_GetError()) + '\n'
                );
                break;
            }

            if (SDLNet_SocketReady(clientSocket))
            {
                int received = SDLNet_TCP_Recv(clientSocket, buffer, MESSAGE_BUFFER_SIZE);

                if (received <= 0)
                {
                    ERROR_LOG("Client disconnected or error receving data.\n");
                    break;
                }

                SDLNet_TCP_Send(clientSocket, buffer, received);
                NOTIFY_LOG("Receive and sent back: " + std::string(buffer) + '\n');
            }
        }

        SDLNet_TCP_DelSocket(serverSocketSet, clientSocket);
        SDLNet_TCP_Close(clientSocket);
    }
}

void serverExitHandle(int)
{
    system("cls");
    printSplitLine(30, '-');
    NOTIFY_LOG("Server: Free TCP Socket Set.\n");
    SDLNet_TCP_DelSocket(serverSocketSet, serverSocket);
    SDLNet_FreeSocketSet(serverSocketSet);

    NOTIFY_LOG("Server: Close TCP Socket.\n");
    SDLNet_TCP_Close(serverSocket);

    NOTIFY_LOG("Server: SDL Net Quit.\n");
    SDLNet_Quit();

    CORRECT_LOG("Server Exit.\n");
    printSplitLine(30, '-');
    exit(EXIT_SUCCESS);
}