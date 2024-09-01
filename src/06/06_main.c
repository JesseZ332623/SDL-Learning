#include "../../include/06.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    if (!init())
    {
        fprintf(stderr, "Init failed.\n");
        exit(EXIT_SUCCESS);
    }
    else 
    {
        if (!loadMedia("../img/06/viewpoint.png"))
        {
            fprintf(stderr, "Faild to load media!\n");
        }
        else
        {
            struct EventsInfo eventInfo;
            memset(&eventInfo, 0, sizeof(struct EventsInfo));

            while (!eventInfo.ifQuit)
            {
                eventControl(&eventInfo);
                rander();
            }
        }
    }

    freeResource();

    return EXIT_SUCCESS;
}
