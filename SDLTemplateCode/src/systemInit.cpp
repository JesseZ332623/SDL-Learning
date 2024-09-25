#include "../include/systemInit.hpp"
#include "../include/fmtTime.hpp"

#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

#include <stdexcept>

#define SDL_INIT_FLAGS (SDL_INIT_VIDEO | SDL_INIT_AUDIO)
#define IMG_INIT_FLAGS (IMG_INIT_PNG | IMG_INIT_JPG)
#define MIX_INIT_FLAGS (MIX_INIT_FLAC | MIX_INIT_MP3)

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

void SystemInit::SDLMixerInit(void)
{
    if (!((Mix_Init(MIX_INIT_FLAGS) & MIX_INIT_FLAGS)))
    {
        throw std::runtime_error(
            "Faild to Init SDL Mixer moudle, SDL MIXER ERROR: " + 
            std::string(Mix_GetError())
        );
    }
}

void SystemInit::gameControllerInit(void)
{
    using namespace fmt;

    // 初始化游戏控制器子系统
    if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
    {
        throw std::runtime_error(
            "Failed to Init SDL game controller subsytem, SDL ERROR: " + 
            std::string(SDL_GetError()) + '\n'
        );
    }

# if false
    // 获取当前连接本机的手柄数量
    int controllerCount = SDL_NumJoysticks();

    // 若没有连接任何手柄，免去后面的操作，直接返回即可。
    if (controllerCount == 0) { 

        print(
            fg(terminal_color::yellow), 
            "{} No joysticks connected.\n", CurrentTime()
        );
        return;
    }

    /**
     * @brief 遍历所有连接本机的手柄（Joysticks），
     *        找到所有游戏控制器设备并交由 gameControllers 动态数组管理。 
    */
    for (int index = 0; index < controllerCount; ++index)
    {
        if (SDL_IsGameController(index)) 
        {
            SDL_GameController * tempGameController = nullptr;
            tempGameController = SDL_GameControllerOpen(index);

            if (!tempGameController) 
            {
                throw std::runtime_error(
                    "Failed to Open this game controller, index = " + std::to_string(index) + 
                    " SDL ERROR: " + std::string(SDL_GetError()) + '\n'
                );
            }
            else {
                this->gameControllers.push_back(tempGameController);
            }
        }
    }
#endif
}

void SystemInit::createWindow()
{
    if (this->windowSize.h == 0 || this->windowSize.w == 0) {
        throw std::runtime_error("Window size must not be 0.\n");
    }
    else if (this->windowName.size() == 0) {
        this->windowName = "Unknow Name";
    }

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
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
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

    try 
    {
        print(
            fg(terminal_color::blue), 
            "{} Init SDL main moudle.\n", CurrentTime()
        );
        
        this->SDLMainInit();
    }
    catch(const std::runtime_error & __except) 
    {
        print(
            fg(color::red) | emphasis::bold, "{} {}\n", 
            CurrentTime(), __except.what()
        );

        exit(EXIT_FAILURE);
    }

    try 
    {
        print(
            fg(terminal_color::blue), 
            "{} Init SDL mix moudle.\n", CurrentTime()
        );

        this->SDLMixerInit();
    }
    catch (const std::runtime_error & __except)
    {
        print(
            fg(color::red) | emphasis::bold, "{} {}\n", 
            CurrentTime(), __except.what()
        );

        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    try 
    {
        print(
            fg(terminal_color::blue), 
            "{} Init SDL subsystem of game controller.\n", CurrentTime()
        );

        this->gameControllerInit();
    }
    catch (const std::runtime_error & __except) 
    {
        print(
            fg(color::red) | emphasis::bold, "{} {}\n", 
            CurrentTime(), __except.what()
        );

        Mix_Quit();
        SDL_Quit();
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

        Mix_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    try 
    {
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
        Mix_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    try 
    {
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
        Mix_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    try 
    {
        print(
            fg(terminal_color::blue),
            "{} Init SDL TTF module.\n", CurrentTime()
        );
        this->SDLTTFInit();

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
        Mix_Quit();
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
        "{} SDL Mixer moudle quie.\n", CurrentTime()
    );
    Mix_CloseAudio();
    Mix_Quit();

    print(
        fg(color::green),
        "{} SDL main module quit.\n", CurrentTime()
    );
    SDL_Quit();
}