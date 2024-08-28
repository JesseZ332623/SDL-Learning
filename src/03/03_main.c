#include "../../include/03.h"

#include <string.h>

int main(int argc, char const *argv[])
{
    if (!init())
    {
        fprintf(stderr, "Init failed.\n");
        exit(EXIT_SUCCESS);
    }
    else 
    {
        if (!loadMedia())
        {
            fprintf(stderr, "Faild to load media!\n");
        }
        else
        {
            struct EventsInfo eventInfo;
            memset(&eventInfo, 0, sizeof(struct EventsInfo));

            while (!eventInfo.ifQuit)
            {
                events(&eventInfo);
                rander();
            }
        }
    }

    close();

    return EXIT_SUCCESS;
}
