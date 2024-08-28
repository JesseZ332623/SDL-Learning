#include "SDL.h"
#include "SDL_image.h"

#include "../../include/01.h"

SDL_Window *Window = NULL;
SDL_Surface *ScreenSurface = NULL;
SDL_Surface *ImageSurface = NULL;

bool init(void)
{
    bool ifSuccess = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL Init failed, ERROR: %s\n", SDL_GetError());
        ifSuccess = false;
    }
    else
    {
        /**
         * @brief SDL_CreateWindow 形参描述：
         *        窗口名，窗口相对于屏幕的位置（x, y）
         *        窗口自身的长宽（w, h）
         *        窗口标志位
         *
         */
        Window = SDL_CreateWindow(
            "01", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);

        if (!Window)
        {
            fprintf(stderr, "Create window failed, ERROR: %s\n", SDL_GetError());
            ifSuccess = false;
        }
        else
        {
            // 获取与这个屏幕相关联的平面，往后会在这个平面上绘制图像。
            ScreenSurface = SDL_GetWindowSurface(Window);
            if (!ScreenSurface)
            {
                fprintf(stderr, "Get window surface failed, ERROR: %s\n", SDL_GetError());
                ifSuccess = false;
            }
        }
    }

    return ifSuccess;
}

bool loadMedia(void)
{
    bool ifSuccess = true;
    const char * imagePath = "../img/01/00020.bmp";

    ImageSurface = SDL_LoadBMP(imagePath);

    if (!ImageSurface)
    {
        fprintf(stderr, "Unable to load image %s, ERROR: %s\n", imagePath, SDL_GetError());
        ifSuccess = false;
    }

    return ifSuccess;
}

void events(struct EventsInfo *__eventInfo)
{
    if (!__eventInfo)
    {
        fprintf(stderr, "events(NULL): __eventInfo is null pointer!\n");
        exit(EXIT_FAILURE);
    }
    static SDL_Event events;

    while (!__eventInfo->ifQuit)
    {
        // 使用循环不停的监听事件并记录到 events 上
        while (SDL_PollEvent(&events) != 0)
        {
            switch (events.type)
            {
                // 点击窗口上 X 按钮时
                case SDL_QUIT:
                    __eventInfo->ifQuit = true;
                    break;

                default:
                    break;
            }
        }
    }
}

void rander(void)
{
    /**
     * @brief SDL_BlitSurface 用于将源平面加载到目标平面，
     *        此处是将图片的平面信息，加载到窗口的平面信息上。
     */
    SDL_BlitSurface(ImageSurface, NULL, ScreenSurface, NULL);

    // 将窗口和该窗口所关联的平面信息加载到屏幕。
    SDL_UpdateWindowSurface(Window);
}

void close(void)
{
    SDL_FreeSurface(ImageSurface);
    SDL_DestroyWindow(Window);

    Window = NULL;
    ScreenSurface = NULL;

    SDL_Quit();
}