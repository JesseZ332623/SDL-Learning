
#include <stdlib.h>
#include <string.h>

#include "../../include/02.h"

int main(int argc, char const *argv[])
{
    system("cls");
    
    if (!init()) {
        fprintf(stderr, "Faild to init!\n");
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
