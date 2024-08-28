#include "../../include/04.h"

#include <string.h>

#define PATH_LENGTH 128

bool renderPathChange(char * __path, struct EventsInfo * __eventsInfo);

int main(int argc, char const *argv[])
{

    char * imagePath = (char *)calloc(PATH_LENGTH, sizeof(char));
    
    if (!init())
    {
        fprintf(stderr, "Init SDL module failed.\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        if (!loadMedia("../img/03/Light_Of_Million_Hopes.png", FIRST_LOAD))
        {
            fprintf(stderr, "Failed to load media.\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            struct EventsInfo eventsInfo;
            memset(&eventsInfo, 0, sizeof(struct EventsInfo));

            while (!eventsInfo.ifQuit)
            {
                eventControl(&eventsInfo);
                rander();

                if (eventsInfo.ifChangeImage)
                {
                    renderPathChange(imagePath, &eventsInfo);
                    if (!loadMedia(imagePath, RELOAD))
                    {
                        fprintf(
                            stderr,"[%s] is a invalid image path, press [C] to try again:\n",
                            imagePath
                        );
                        printSplitLine(65, '-');
                        memset(imagePath, 0, PATH_LENGTH);
                        continue;
                    }
                    else
                    {
                        printf("Render image: %s\n", imagePath);
                        printSplitLine(65, '-');
                    }
                }
            }
        }
    }

    free(imagePath);
    freeResource();

    return EXIT_SUCCESS;
}

bool renderPathChange(char * __path, struct EventsInfo * __eventsInfo)
{
    if (!__path)
    {
        fprintf(stderr, "renderPathChange(NULL): invalid arguiment.\n");
        return false;
    }
    if (!__eventsInfo)
    {
        fprintf(stderr, "renderPathChange(..., NULL): invalid arguiment.\n");
        return false;
    }

    printf("Enter new image path: (format: <.png> <.jpg>, path length <= [%d] character):\n", PATH_LENGTH);
    fgets(__path, PATH_LENGTH, stdin);
    *strchr(__path, '\n') = '\0';

    __eventsInfo->ifChangeImage = false;

    printSplitLine(65, '-');

    return true;
}
