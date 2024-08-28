#include "../../include/05.h"

int main(int argc, char const *argv[])
{
    if (!init())
    {
        fprintf(stderr, "Init SDL moudle failed.\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        struct EventsInfo info = {0};

        while (!info.ifQuit)
        {
            eventControl(&info);
            rander();
        }
    }

    freeResource();
    
    return EXIT_SUCCESS;
}
