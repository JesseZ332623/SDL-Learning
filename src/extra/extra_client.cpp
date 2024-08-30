#include "./defs.hpp"

#include <csignal>

#define PORT                8888    // 端口号
#define MESSAGE_BUFFER_SIZE 512     // 数据的字节数

using namespace MyLib::MyLoger;

int main(int argc, char *argv[])
{
    std::signal(SIGINT, clientExitHandle);

    system("cls");

    if (argc != 3) {
        WARNING_LOG("Usage: " + std::string(argv[0]) + " <Server IP> <Port>\n");
        return EXIT_FAILURE;
    }

    const char * serverIP = argv[1];
    int port              = atoi(argv[2]);

    ClientOperator(serverIP, port);

    return EXIT_SUCCESS;
}