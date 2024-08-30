#include "./defs.hpp"

#include <csignal>

int main(int argc, char *argv[])
{
    signal(SIGINT, serverExitHandle);

    system("cls");

    ServerOperator();

    return EXIT_SUCCESS;
}
