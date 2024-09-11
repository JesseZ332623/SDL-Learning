#include "../include/systemInit.hpp"

#include <stdexcept>
#include <MyLib/myLogerDef.h>

#define SDL_INIT_FLAGS (SDL_INIT_VIDEO | SDL_INIT_AUDIO)
#define IMG_INIT_FLAGS (IMG_INIT_PNG | IMG_INIT_JPG)

void SystemInit::SDLMainInit(void)
{
    if (SDL_Init(SDL_INIT_FLAGS) < 0)
    {
        throw std::runtime_error(
            "Failed to Init SDL main module, SDL ERROR: " + 
            std::string(SDL_GetError()) + '\n'
        );
    }
}

void SystemInit::createWindow(const WindowSize & __windowSize)
{
    this->mainWindow = SDL_CreateWindow(
        windowName.c_str(), 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        __windowSize.w, __windowSize.h, SDL_WINDOW_SHOWN
    );

    if (!this->mainWindow)
    {
        throw std::runtime_error(
            "Failed to create window, SDL ERROR: " + 
            std::string(SDL_GetError()) + '\n'
        );
    }
}

void SystemInit::createRenderer(void)
{
    this->render = SDL_CreateRenderer(
        this->mainWindow, -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!this->render)
    {
        throw std::runtime_error(
            "Faile to create renderer, SDL ERROR: " + 
            std::string(SDL_GetError()) + '\n'
        );
    }
}

void SystemInit::SDLImageInit(void)
{
    if (!(IMG_Init(IMG_INIT_FLAGS) & IMG_INIT_FLAGS))
    {
        throw std::runtime_error(
            "Failed to init SDL image module, SDL Image ERROR: " + 
            std::string(SDL_GetError()) + '\n'
        );
    }
}

void SystemInit::init(WindowSize __windowSize, std::string __windowName)
{
    using namespace MyLib::MyLoger;

    try {
        NOTIFY_LOG("Init SDL main moudle.\n");
        this->SDLMainInit();
    }
    catch(const std::runtime_error & __except) {
        ERROR_LOG(__except.what());
        exit(EXIT_FAILURE);
    }
    try {
        this->windowName = __windowName;
        NOTIFY_LOG("Create main window: [" + SystemInit::windowName + "] \n");
        this->createWindow(__windowSize);
    }
    catch(const std::runtime_error & __except)
    {
        ERROR_LOG(__except.what());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    try {
        NOTIFY_LOG("Create renderer.\n");
        this->createRenderer();
    }
    catch (const std::runtime_error & __except)
    {
        ERROR_LOG(__except.what());
        SDL_DestroyWindow(this->mainWindow);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    try {
        NOTIFY_LOG("Init SDL image module.\n");
        this->SDLImageInit();
    }
    catch (const std::runtime_error & __except)
    {
        ERROR_LOG(__except.what());
        SDL_DestroyRenderer(this->render);
        SDL_DestroyWindow(this->mainWindow);
        IMG_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}

SystemInit::~SystemInit()
{
    using namespace MyLib::MyLoger;

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