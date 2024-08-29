#include "../../include/05.h"

#include "SDL.h"
// #include "SDL_image.h"

#include "pcg_basic.h"

struct SDL_Window   * Window  = NULL;
struct SDL_Renderer * Render  = NULL;
struct SDL_Texture  * AlphaTexture = NULL;

// 随机数生成器
static pcg32_random_t randomDeveice;

const char * WindowName = "05";

struct PCG_Initilizer
{
    uint64_t seed_01, seed_02;
};

void printSplitLine(size_t __len, const char __style)
{
    while (__len-- != 0)
    {
        putchar(__style);
    }
    putchar('\n');
}

/**
 * @brief 清空渲染器，并重新把渲染颜色设置成纯白。
*/
static void ScreenClear(struct SDL_Renderer * __renderer);

/**
 * @brief 输出一个 SDL_Rect 的信息
*/
static void showRect(const struct SDL_Rect * __rect);

/**
 * @brief 初始化随机数生成器
 */
static void initRandomDevice(pcg32_random_t * __randomDeveice);

/**
 * @brief 使用 PCG 随机数生成器，
 *        依据 PCG_Initilizer 结构体的种子生成（0 <= randnumber < __bound）范围内的随机数。
 * 
 * @param __bound 生成随机数的最大边界
 * 
 * @return 生成的随机数
*/
static uint32_t getRandomNumber(pcg32_random_t * __randomDeveice, const uint32_t __bound);

/**
 * @brief 调整纹理渲染位置的边界，确保方块是完整的渲染在窗口内的。
*/
static void adjustTextureBound(struct SDL_Rect * __renderPos);

static void ScreenClear(struct SDL_Renderer * __renderer)
{
    if (!__renderer)
    {
        fprintf(
            stderr, "ScreenClear(NULL): Argument __renderer is a NULL pointer!\n"
        );
        exit(EXIT_FAILURE);
    }
    SDL_SetRenderDrawColor(__renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(__renderer);
}

static void showRect(const struct SDL_Rect * __rect)
{
    if (!__rect)
    {
        fprintf(stderr, "showRect(NULL): passed NULL SDL_Rect.\n");
        return;
    }

    printf("(x, y) = (%d, %d)\n", __rect->x, __rect->y);
}

static void initRandomDevice(pcg32_random_t * __randomDeveice)
{
    if (!__randomDeveice)
    {
        fprintf(stderr, "initRandomDevice(NULL): passed NULL pcg32_random_t.\n");
        return;
    }
    struct PCG_Initilizer pcgSeedInit = PCG32_INITIALIZER;

    pcg32_srandom_r(__randomDeveice, pcgSeedInit.seed_01, pcgSeedInit.seed_02);
}

static uint32_t getRandomNumber(pcg32_random_t * __randomDeveice, const uint32_t __bound)
{
    return pcg32_boundedrand_r(__randomDeveice, __bound);
}

static void adjustTextureBound(struct SDL_Rect * __renderPos)
{
    if (!__renderPos) {
        fprintf(stderr, "adjustTextureBound(NULL): Passed NULL SDL_Rect.\n");
        return;
    }

    if (__renderPos->x > SCREEN_WIDTH - __renderPos->w) {
        __renderPos->x = SCREEN_WIDTH - __renderPos->w;
    }

    if (__renderPos->y > SCREEN_HEIGHT - __renderPos->h) {
        __renderPos->y = SCREEN_HEIGHT - __renderPos->h;
    }
}

bool init(void)
{
    bool initSuccess = true;

    initRandomDevice(&randomDeveice);

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
             *        SDL_RENDERER_ACCELERATED  枚举表示使用硬件加速
             *        SDL_RENDERER_PRESENTVSYNC 枚举表示在呈现每一帧时尽可能的同步到显示器的垂直刷新率（垂直同步）
            */
            Render = SDL_CreateRenderer(
                Window, -1, 
                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
            );

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
                AlphaTexture = SDL_CreateTexture(
                                    Render, 
                                    SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET,
                                    SCREEN_WIDTH, SCREEN_HEIGHT
                                );

                if (!AlphaTexture)
                {
                    fprintf(stderr, "Failed to create texture, SDL ERROR: %s\n", SDL_GetError());
                    initSuccess = false;
                }
                else
                {
                    ScreenClear(Render);    // 初始化渲染器渲染颜色
                }
#if false
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
#endif
            }
        }
    }

    return initSuccess;
}

void eventControl(struct EventsInfo * __info)
{
    if (!__info)
    {
        fprintf(stderr, "eventControl(NULL): Argument __info is a NULL pointer.\n");
        exit(EXIT_FAILURE);
    }

    static union SDL_Event events;
    while (SDL_PollEvent(&events))
    {
        if (events.type == SDL_QUIT) {
            __info->ifQuit = true;
        }
    }
}

void rander(void)
{
    // 设置 Render 的渲染目标为纹理。
    SDL_SetRenderTarget(Render, AlphaTexture);
    ScreenClear(Render);

    /**
     * @brief 设置纹理的大小，
     *        它的位置在 (0, 0)，以窗口左上角为原点 (0, 0) 
     *        它的长宽皆为 100 像素，
    */
    struct SDL_Rect renderPos = {0};

    renderPos.x = getRandomNumber(&randomDeveice, (uint32_t)SCREEN_WIDTH)  + 1;
    renderPos.y = getRandomNumber(&randomDeveice, (uint32_t)SCREEN_HEIGHT) + 1;
    renderPos.h = renderPos.w = 100;

    adjustTextureBound(&renderPos);

    showRect(&renderPos);

    /**
     * @brief SDL_SetRenderDrawColor() 设置渲染器的渲染颜色
     *        SDL_RenderFillRect()     把纹理的位置信息提交给渲染器
    */
    SDL_SetRenderDrawColor(Render, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(Render, &renderPos);

    // 切换渲染目标为窗口
    SDL_SetRenderTarget(Render, NULL);

    // 拷贝纹理信息至渲染器
    SDL_RenderCopy(Render, AlphaTexture, NULL, NULL);
    SDL_RenderPresent(Render);      // 更新屏幕

    delay(35);
}

void freeResource(void)
{
    SDL_DestroyTexture(AlphaTexture);
    SDL_DestroyRenderer(Render);
    SDL_DestroyWindow(Window);

    SDL_Quit();
}