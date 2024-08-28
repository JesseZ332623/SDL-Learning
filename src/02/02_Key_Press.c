#include "../../include/02.h"

#include "SDL.h"
#include "SDL_image.h"

const char * imagePath[KEY_PRESS_SURFACE_TOTAL] = 
{
    "../img/02/press.bmp",
    "../img/02/up.bmp",
    "../img/02/down.bmp",
    "../img/02/left.bmp",
    "../img/02/right.bmp",
};

SDL_Window  * Window          = NULL;   // 窗口类
SDL_Surface * ScreenSurface   = NULL;   // 窗口平面信息

// 保存了不同方向键头图片平面信息的指针数组
SDL_Surface * KeyPressSurface[KEY_PRESS_SURFACE_TOTAL];

// 当前应当渲染的键头图片
SDL_Surface * CurrentSurface = NULL;

/**
 * @brief 加载指定路径图片的平面信息并返回。
*/
static SDL_Surface * loadSurface(const char * __imagePath);

static SDL_Surface * loadSurface(const char * __imagePath)
{
    // 一个原始的 BMP 文件比较如果加载的图片很多，会影响性能。
    SDL_Surface * loadedSurface    = SDL_LoadBMP(__imagePath);

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
    bool ifLoadSuccess = true;

    // 加载所有的图片平面信息
    for (size_t index = 0UL; index < KEY_PRESS_SURFACE_TOTAL; ++index)
    {
        printf("Load image: [%s]\n", imagePath[index]);

        KeyPressSurface[index] = loadSurface(imagePath[index]);

        if (!KeyPressSurface[index])
        {
            fprintf(
                stderr, "Failed to load image: [%s], ERROR: %s\n",
                imagePath[index], SDL_GetError()
            );
            ifLoadSuccess = false;
        }
    }

    // 设置当前待渲染的图片
    CurrentSurface = KeyPressSurface[KEY_PRESS_SURFACE_DEFAULT];

    return ifLoadSuccess;
}

void events(struct EventsInfo * __eventInfo)
{
# if false
    if (!__eventInfo)
    {
        fprintf(stderr, "events(NULL): __eventInfo is null pointer!\n");
        exit(EXIT_FAILURE);
    }
#endif

    static SDL_Event events;
    
    while (SDL_PollEvent(&events) != 0)
    {
        if (events.type == SDL_QUIT) { __eventInfo->ifQuit = true; }

        else if (events.type == SDL_KEYDOWN)
        {
            switch (events.key.keysym.sym)
            {
                case SDLK_UP:
                    CurrentSurface = KeyPressSurface[KEY_PRESS_SURFACE_UP];
                    break;
                
                case SDLK_DOWN:
                    CurrentSurface = KeyPressSurface[KEY_PRESS_SURFACE_DOWN];
                    break;
                
                case SDLK_LEFT:
                    CurrentSurface = KeyPressSurface[KEY_PRESS_SURFACE_LEFT];
                    break;
                
                case SDLK_RIGHT:
                    CurrentSurface = KeyPressSurface[KEY_PRESS_SURFACE_RIGHT];
                    break;

                default:
                    CurrentSurface = KeyPressSurface[KEY_PRESS_SURFACE_DEFAULT];
                    break;
            }
        }
    }
    
}

void rander(void)
{
    SDL_Rect stretchRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_BlitScaled(CurrentSurface, NULL, ScreenSurface, &stretchRect);
    SDL_UpdateWindowSurface(Window);
}

void close(void)
{
    for (size_t index = 0UL; index < KEY_PRESS_SURFACE_TOTAL; ++index)
    {
        printf("Free key press surface: [%s].\n", imagePath[index]);
        SDL_FreeSurface(KeyPressSurface[index]);
    }

    memset(KeyPressSurface, 0, KEY_PRESS_SURFACE_TOTAL * sizeof(SDL_Surface *));
    CurrentSurface = NULL;

    printf("Destroy Window ID = %u.\n", SDL_GetWindowID(Window));
    SDL_DestroyWindow(Window);
    ScreenSurface = NULL;
    Window        = NULL;

    printf("SDL Quit.\n");
    SDL_Quit();
}