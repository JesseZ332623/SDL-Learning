#include "../include/systemInit.hpp"
#include "../include/fmtTime.hpp"

#include "SDL_image.h"
#include "SDL_ttf.h"

#include <stdexcept>

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

void SystemInit::createWindow()
{
    this->mainWindow = SDL_CreateWindow(
        this->windowName.c_str(), 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        this->windowSize.w, this->windowSize.h, SDL_WINDOW_SHOWN
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

void SystemInit::SDLTTFInit(void)
{
    if (TTF_Init() == -1)
    {
        throw std::runtime_error(
            "Failed to init SDL TTF module, SDL Image ERROR: " + 
            std::string(SDL_GetError()) + '\n'
        );
    }
}

SystemInit::SystemInit(WindowSize __windowSize, std::string __windowName) : SystemInit()
{
    this->windowSize = __windowSize;
    this->windowName = __windowName;
}

void SystemInit::init(WindowSize __windowSize, std::string __windowName)
{
    using namespace fmt;
    using fmt::terminal_color;

    try {
        print(
            fg(terminal_color::blue), 
            "{} Init SDL main moudle.\n", CurrentTime()
        );
        
        this->SDLMainInit();
    }
    catch(const std::runtime_error & __except) {
        print(
            fg(color::red) | emphasis::bold, "{} {}\n", 
            CurrentTime(), __except.what()
        );

        exit(EXIT_FAILURE);
    }
    try {
        print(
            fg(terminal_color::blue),
            "{} Create main window: [{}]\n", 
            CurrentTime(), SystemInit::windowName
        );

        if (!this->windowName.size()) {
            this->windowName = __windowName;
            this->windowSize = __windowSize;
        }
        
        this->createWindow();
    }
    catch(const std::runtime_error & __except)
    {
        print(
            fg(color::red) | emphasis::bold, "{} {}\n", 
            CurrentTime(), __except.what()
        );

        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    try {
        print(
            fg(terminal_color::blue),
            "{} Create renderer.\n", CurrentTime()
        );
        
        this->createRenderer();
    }
    catch (const std::runtime_error & __except)
    {
        print(
            fg(color::red) | emphasis::bold, "{} {}\n", 
            CurrentTime(), __except.what()
        );

        SDL_DestroyWindow(this->mainWindow);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    try {
        print(
            fg(terminal_color::blue),
            "{} Init SDL image module.\n", CurrentTime()
        );

        this->SDLImageInit();
    }
    catch (const std::runtime_error & __except)
    {
        print(
            fg(color::red) | emphasis::bold, "{} {}\n", 
            CurrentTime(), __except.what()
        );

        SDL_DestroyRenderer(this->render);
        SDL_DestroyWindow(this->mainWindow);
        IMG_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    try {
        print(
            fg(terminal_color::blue),
            "{} Init SDL TTF module.\n", CurrentTime()
        );
        this->SDLTTFInit();

    } catch (const std::runtime_error & __except) {
        print(
            fg(color::red) | emphasis::bold, "{} {}\n", 
            CurrentTime(), __except.what()
        );

        SDL_DestroyRenderer(this->render);
        SDL_DestroyWindow(this->mainWindow);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}

SystemInit::~SystemInit()
{
    using namespace fmt;
    using fmt::terminal_color;

    print(
        fg(color::green),
        "{} Destroy renderer.\n", CurrentTime()
    );
    SDL_DestroyRenderer(this->render);
    this->render = nullptr;

    print(
        fg(color::green),
        "{} Destroy window: [{}].\n", 
        CurrentTime(), this->windowName
    );
    SDL_DestroyWindow(this->mainWindow);
    this->mainWindow = nullptr;

    print(
        fg(color::green),
        "{} SDL image module quit.\n", CurrentTime()
    );
    IMG_Quit();

    print(
        fg(color::green),
        "{} SDL TTF module quit.\n", CurrentTime()
    );
    TTF_Quit();

    print(
        fg(color::green),
        "{} SDL main module quit.\n", CurrentTime()
    );
    SDL_Quit();
}