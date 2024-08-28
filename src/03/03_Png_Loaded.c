#include "../../include/03.h"

#include "SDL.h"
#include "SDL_image.h"

SDL_Window  * Window          = NULL;   // 窗口类
SDL_Surface * ScreenSurface   = NULL;   // 窗口平面信息

SDL_Surface * imageSurface    = NULL;

/**
 * @brief 加载指定路径图片的平面信息并返回。
*/
static SDL_Surface * loadSurface(const char * __imagePath);

static SDL_Surface * loadSurface(const char * __imagePath)
{
    // 调用 IMG_Load() 去加载 png, jpg 格式的图片
    SDL_Surface * loadedSurface    = IMG_Load(__imagePath);

    // 所以在读取完原始的图片后，会将其优化后在使用。
    SDL_Surface * optimizedSurface = NULL;

    if (!loadedSurface)
    {
        fprintf(
            stderr, "Load surface of [%s] failed, ERROR: %s\n",
            __imagePath, SDL_GetError()
        );
    }
    else
    {
        /**
         * @brief 不同的显示器支持的颜色位深度可能不同（32，24，16 皆有之），所以在渲染图片前，
         *        先调用 SDL_ConvertSurface() 将原始图片的相关属性转换成完全兼容显示器的格式，
         *        避免渲染时进行不必要的数据转换带来额外的开销。
        */
        optimizedSurface = SDL_ConvertSurface(loadedSurface, ScreenSurface->format, 0);
        if (!optimizedSurface)
        {
            fprintf(
                stderr, "Unable to optimize image %s, ERROR: %s",
                __imagePath, SDL_GetError()
            );
        }

        SDL_FreeSurface(loadedSurface);
    }

    return optimizedSurface;
}

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
            "02", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);

        if (!Window)
        {
            fprintf(stderr, "Create window failed, ERROR: %s\n", SDL_GetError());
            ifSuccess = false;
        }
        else
        {
            // 初始化 SDL_Image 的 PNG 文件读取模块。
            int imgInitFlag = (IMG_INIT_PNG | IMG_INIT_JPG);
            if (!(IMG_Init(imgInitFlag) & imgInitFlag))
            {
                fprintf(stderr, "Failed to init SDL_Image, ERROR: %s\n", SDL_GetError());
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
    }

    return ifSuccess;
}

bool loadMedia(void)
{
    bool ifLoadedSuccess = true;
    const char * imagePath = "../img/03/Light_Of_Million_Hopes.png";

    imageSurface = loadSurface(imagePath);
    if (!imageSurface)
    {
        fprintf(
            stderr, "Failed to load surface from: [%s], ERROR: %s\n",
            imagePath, SDL_GetError()
        );
        ifLoadedSuccess = false;
    }

    return ifLoadedSuccess;
}

void events(struct EventsInfo * __eventInfo)
{
    static SDL_Event events;

    while (SDL_PollEvent(&events) != 0)
    {
        switch (events.type)
        {
            case SDL_QUIT:
                __eventInfo->ifQuit = true;
                break;

            default:
                break;
        }
    }
}

void rander(void)
{
    SDL_Rect Rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_BlitScaled(imageSurface, NULL, ScreenSurface, &Rect);
    SDL_UpdateWindowSurface(Window);
}

void close(void)
{
    SDL_FreeSurface(imageSurface);
    imageSurface = NULL;

    SDL_DestroyWindow(Window);
    ScreenSurface = NULL;
    Window = NULL;

    IMG_Quit();
    SDL_Quit();
}