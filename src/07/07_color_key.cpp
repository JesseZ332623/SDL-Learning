#include "../../include/07.hpp"

using namespace MyLib::MyLoger;

#define IMG_INIT_FLAGS (IMG_INIT_PNG | IMG_INIT_JPG)

const int SCREEN_HEIGHT = 480;
const int SCREEN_WIDTH  = 640;

const char * WINDOW_NAME = "07. Color Key";

void SystemInit::init(void)
{
    NOTIFY_LOG("Init SDL main moudle.\n");
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        ERROR_LOG(
            "Failed to init SDL main module, SDL ERROR: " +
            std::string(SDL_GetError()) + '\n'
        );
        exit(EXIT_FAILURE);
    }
    else
    {
        NOTIFY_LOG("Create main window: [" + std::string(WINDOW_NAME) + "] \n");
        this->mainWindow = SDL_CreateWindow(
            WINDOW_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
        );

        if (!this->mainWindow)
        {
            ERROR_LOG(
                "Failed to create window, SDL ERROR: " +
                std::string(SDL_GetError()) + '\n'
            );

            SDL_Quit();
            exit(EXIT_FAILURE);
        }
        else
        {
            NOTIFY_LOG("Create renderer.\n");
            this->render = SDL_CreateRenderer(
                this->mainWindow, -1, 
                SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED
            );

            if (!render)
            {
                ERROR_LOG(
                    "Failed to create renderer, SDL ERROR: " + 
                    std::string(SDL_GetError()) + '\n'
                );

                SDL_DestroyWindow(this->mainWindow);
                SDL_Quit();
                exit(EXIT_FAILURE);
            }
            else
            {
                NOTIFY_LOG("Init SDL image module.\n");
                if (!(IMG_Init(IMG_INIT_FLAGS) & IMG_INIT_FLAGS))
                {
                    ERROR_LOG(
                        "Failed to init SDL image moudle, SDL ERROR: " + 
                        std::string(SDL_GetError()) + '\n'
                    );

                    SDL_DestroyRenderer(this->render);
                    SDL_DestroyWindow(this->mainWindow);
                    SDL_Quit();
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
}

SystemInit::~SystemInit()
{
    CORRECT_LOG("Destroy renderer.\n");
    SDL_DestroyRenderer(this->render);
    this->render = nullptr;

    CORRECT_LOG("Destory window.\n");
    SDL_DestroyWindow(this->mainWindow);
    this->mainWindow = nullptr;

    CORRECT_LOG("SDL image module quit.\n");
    IMG_Quit();

    CORRECT_LOG("SDL main module quit.\n");
    SDL_Quit();
}

void Texture::freeTexture(void)
{
    if (!this->texture)
    {
        SDL_DestroyTexture(this->texture);
        this->texture = nullptr;
        this->width = this->height = 0;
    }
}

bool Texture::loadFromFile(std::string __path, SDL_Renderer * __render)
{
    this->freeTexture();

    SDL_Texture * finalTexture = nullptr;

    NOTIFY_LOG("Load image [" + __path + "]\n");
    SDL_Surface * loadSurface = IMG_Load(__path.c_str());

    if (!loadSurface)
    {
        ERROR_LOG(
            "Unable to load image: " + __path + 
            " SDL Image ERROR: " + std::string(IMG_GetError()) + '\n'
        );
    }
    else
    {
        /**
         * @brief SDL_SetColorKey() 函数的作用是将一张图片的指定像素设置成透明像素，
         *        并在之后的渲染中忽视这些像素，函数需要如下几个参数。
         *        
         *        1. 一张图片的平面信息，通过 IMG_Load() 获得
         *        2. 一个 SDL 布尔类型，表明是否启用该色键
         *        3. 像素点键值，使用 SDL_MapRGB() 或 SDL_MapRGBA() 来获取，
         *           上面两个函数通过传入平面的像素格式与颜色值，
         *           将这些参数映射成像素值交给外部去处理。
        */
        SDL_SetColorKey(
            loadSurface, SDL_TRUE, 
            SDL_MapRGBA(loadSurface->format, 0x00, 0xFF, 0xFF, 0xFF)
        );

        finalTexture = SDL_CreateTextureFromSurface(__render, loadSurface);

        if (!finalTexture)
        {
            ERROR_LOG(
                "Unable to create texture from " + __path + 
                " SDL ERROR: " + std::string(SDL_GetError()) + '\n'
            );
        }
        else {
            this->width  = loadSurface->w;
            this->height = loadSurface->h;
        }

        SDL_FreeSurface(loadSurface);
    }

    this->texture = finalTexture;

    return (texture != nullptr);
}

void Texture::render(int __x, int __y, SDL_Renderer * __render)
{
    this->renderPosition.w = this->width;
    this->renderPosition.h = this->height;
    this->renderPosition.x = __x;
    this->renderPosition.y = __y;

    SDL_RenderCopy(__render, this->texture, nullptr, &this->renderPosition);
}

void EventsControl::events(void)
{
    while (SDL_PollEvent(&this->sdlEvents) != 0)
    {
        if (this->sdlEvents.type == SDL_QUIT) {
            this->info.quit = true;
        }
    }
}