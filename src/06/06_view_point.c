#include <stdio.h>
#include <stdlib.h>

#include "../../include/06.h"

#include "SDL.h"
#include "SDL_image.h"

struct SDL_Window   * Window  = NULL;
struct SDL_Renderer * Render  = NULL;
struct SDL_Texture  * Texture = NULL;

const char * WindowName = "06";

void printSplitLine(size_t __len, const char __style)
{
    while (__len-- != 0)
    {
        putchar(__style);
    }
    putchar('\n');
}

/**
 * @brief 加载指定图片的纹理信息并返回。
*/
static SDL_Texture * loadTexture(const char * __imagePath);

bool init(void)
{
    bool initSuccess = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Init SDL failed, SDL ERROR: %s.\n", SDL_GetError());
        initSuccess = false;
    }
    else
    {
        Window = SDL_CreateWindow(
            WindowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
        );

        if (!Window)
        {
            fprintf(
                stderr, "Create window %s failed, SDL ERROR: %s\n",
                WindowName, SDL_GetError()
            );
            initSuccess = false;
        }
        else
        {
            /**
             * @brief 基于一个窗口创建渲染器，
             *        index 参数为 -1 意味着初始化第一个支持请求标志的对象
             *        SDL_RENDERER_ACCELERATED  枚举表示使用硬件加速，
             *        SDL_RENDERER_PRESENTVSYNC 枚举意为垂直同步。
            */
            Render = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

            if (!Render)
            {
                fprintf(
                    stderr, "Create renderer (Based on Window %s) failed, ERROR: %s\n",
                    WindowName, SDL_GetError()
                );
                initSuccess = false;
            }
            else
            {
                /**
                 * @brief 为指定渲染器设置渲染颜色，使用 RGBA 来指定，
                 *        该函数可以设置渲染器在各种渲染操作中（如渲染矩形，线，点）使用何种颜色。
                */
                SDL_SetRenderDrawColor(Render, 0xFF, 0xFF, 0xFF, 0xFF);

                /**
                 * @brief IMG_Init() 初始化图片加载模块，返回当前所有加载图片格式的标志位，
                 *        当前是 ((IMG_INIT_PNG) | (IMG_INIT_JPG))
                 * 
                */
                if (!(IMG_Init(IMAGE_INIT_FLAGS) & IMAGE_INIT_FLAGS))
                {
                    fprintf(
                        stderr, "Faild to init SDL_Image, SDL ERROR: %s.\n", 
                        SDL_GetError()
                    );
                    initSuccess = false;
                }
            }
        }
    }

    return initSuccess;
}

static SDL_Texture * loadTexture(const char * __imagePath)
{
    if (!__imagePath || !(*__imagePath))
    {
        fprintf(stderr, "loadTexture(NULL): Argument is invalid path.\n");
        return NULL;
    }

    SDL_Texture * finalTexture = NULL;

    // 先加载一张图片的平面信息
    SDL_Surface * surface = IMG_Load(__imagePath);

    if (!surface)
    {
        fprintf(
            stderr, "Load surface from %s failed, SDL ERROR: %s\n",
            __imagePath, SDL_GetError()
        );
    }
    else
    {
        // 在通过图片的渲染信息结合渲染器生成图片纹理。
        finalTexture = SDL_CreateTextureFromSurface(Render, surface);

        if (!finalTexture)
        {
            fprintf(
                stderr, "Create texture from %s failed, SDL ERROR: %s\n",
                __imagePath, SDL_GetError()
            );
        }

        SDL_FreeSurface(surface);
    }

    return finalTexture;
}

bool loadMedia(const char * __imagePath)
{
    bool ifSuccess = true;

    if (!__imagePath || !(*__imagePath))
    {
        fprintf(stderr, "loadMedia(NULL): Argument is invalid path.\n");
        ifSuccess = false;
    }

    Texture = loadTexture(__imagePath);

    if (!Texture)
    {
        fprintf(stderr, "Faild to load texture from %s.\n", __imagePath);
        ifSuccess = false;
    }

    return ifSuccess;
}

void eventControl(struct EventsInfo * __info)
{
    if (!__info) {
        fprintf(stderr, "eventControl(NULL): NULL argument.\n");
        exit(EXIT_FAILURE);
    }
    
    static SDL_Event events;

    while (SDL_PollEvent(&events) != 0)
    {
        if (events.type == SDL_QUIT) {
            __info->ifQuit = true;
        }
    }
}

void rander(void)
{
    /**
     * @brief 图片位置（0，0），图片长和宽分别是窗口长宽的一半。
     *        所以渲染在窗口的左上。
    */
    static SDL_Rect viewpointLeftTop  = {0, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};

    /**
     * @brief 图片位置（SCREEN_WIDTH / 2，0），图片长和宽分别是窗口长宽的一半。
     *        所以渲染在窗口的右上。
    */
    static SDL_Rect viewpointRightTop = {SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};

    /**
     * @brief 图片位置（SCREEN_WIDTH / 2，0），图片宽是窗口长宽的一半，图片长就是窗口的长度。
     *        所以占满了下半个屏幕。
    */
    static SDL_Rect viewpointButtom   = {0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2};

    SDL_RenderClear(Render);

    /**
     * @brief 将位置信息设置后提交给渲染器
    */
    SDL_RenderSetViewport(Render, &viewpointLeftTop);
    SDL_RenderCopy(Render, Texture, NULL, NULL);
    SDL_RenderSetViewport(Render, &viewpointRightTop);
    SDL_RenderCopy(Render, Texture, NULL, NULL);
    SDL_RenderSetViewport(Render, &viewpointButtom);
    SDL_RenderCopy(Render, Texture, NULL, NULL);
    
    // 所以位置，纹理信息提交完毕后，更新屏幕。
    SDL_RenderPresent(Render);
}

void freeResource(void)
{
    SDL_DestroyTexture(Texture);
    SDL_DestroyRenderer(Render);
    SDL_DestroyWindow(Window);

    Texture = NULL;
    Render  = NULL;
    Window  = NULL;

    IMG_Quit();
    SDL_Quit();
}