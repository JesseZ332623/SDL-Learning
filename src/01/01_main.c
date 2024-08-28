#include "../../include/01.h"

#include <string.h>

int main(int argc, char const *argv[])
{
    // 初始化 SDL 等资源
    if (!init()) {
        fprintf(stderr, "Faild to init!\n");
    }
    else 
    {
        // 加载图像资源
        if (!loadMedia()) 
        {
            fprintf(stderr, "Load media failed!\n");
        }
        else 
        {
            struct EventsInfo eventsInfo;
            memset(&eventsInfo, 0, sizeof(struct EventsInfo));

            rander();   // 渲染图像资源
            events(&eventsInfo);
            
            if (eventsInfo.ifQuit)
            {
                printf("Quit signal detected.\n");
            }
        }
    }
    
    close();

    return EXIT_SUCCESS;
}
