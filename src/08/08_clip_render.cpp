
#include "../../include/08.hpp"
#include <cstring>

#define SDL_INIT_FLAGS (SDL_INIT_VIDEO | SDL_INIT_AUDIO)
#define IMG_INIT_FLAGS (IMG_INIT_PNG | IMG_INIT_JPG)

std::string SystemInit::windowName = "08. Clip Render";

void SystemInit::SDLMainInit(void)
{
    if (SDL_Init(SDL_INIT_FLAGS) < 0)
    {
        throw std::runtime_error(
            "Failed to Init SDL main module, SDL ERROR: " + std::string(SDL_GetError()) + '\n'
        );
    }
}

void SystemInit::createWindow(void)
{
    this->mainWindow = SDL_CreateWindow(
        windowName.c_str(), 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
    );

    if (!this->mainWindow)
    {
        throw std::runtime_error(
            "Failed to create window, SDL ERROR: " + std::string(SDL_GetError()) + '\n'
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
            "Faile to create renderer, SDL ERROR: " + std::string(SDL_GetError()) + '\n'
        );
    }
}

void SystemInit::SDLImageInit(void)
{
    if (!(IMG_Init(IMG_INIT_FLAGS) & IMG_INIT_FLAGS))
    {
        throw std::runtime_error(
            "Failed to init SDL image module, SDL Image ERROR: " + std::string(SDL_GetError()) + '\n'
        );
    }
}

void SystemInit::init(void)
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
        NOTIFY_LOG("Create main window: [" + SystemInit::windowName + "] \n");
        this->createWindow();
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

void Texture::freeTexture(void)
{
    if (!this->texture)
    {
        SDL_DestroyTexture(this->texture);
        this->texture = nullptr;
        std::memset(&this->clipPosition, 0, sizeof(SDL_Rect));
        std::memset(&this->renderPosition, 0, sizeof(SDL_Rect));
    }
}

inline void Texture::setClipPosition(SDL_Rect & __clipPos)
{
    std::memcpy(&this->clipPosition, &__clipPos, sizeof(SDL_Rect));
}

bool Texture::loadFromFile(std::string __path, SDL_Renderer * __render)
{
    using namespace MyLib::MyLoger;

    SDL_Texture * finalTexture = nullptr;

    NOTIFY_LOG("Load image [" + __path + "]\n");
    SDL_Surface * loadSurface = IMG_Load(__path.c_str());

    if (!loadSurface) {
        ERROR_LOG("Unable to load image: " + __path + 
                  " SDL Image ERROR: " + std::string(IMG_GetError()) + '\n'
        );
    }
    else
    {
        SDL_SetColorKey(
            loadSurface, SDL_TRUE, 
            SDL_MapRGB(loadSurface->format, 0x00, 0xFF, 0x00)
        );

        finalTexture = SDL_CreateTextureFromSurface(__render, loadSurface);

        if (!finalTexture) {
            ERROR_LOG(
                "Unable to create texture from " + __path + 
                " SDL ERROR: " + std::string(SDL_GetError()) + '\n'
            );
        }
        else {
            this->renderPosition.w = loadSurface->w;
            this->renderPosition.h = loadSurface->h;
        }

        SDL_FreeSurface(loadSurface);
    }

    this->texture = finalTexture;

    return (this->texture != nullptr);
}

void Texture::render(int __x, int __y, SDL_Rect __clipPos, SDL_Renderer * __render)
{
    this->setClipPosition(__clipPos);
    this->renderPosition.x = __x;
    this->renderPosition.y = __y;
    this->renderPosition.w = __clipPos.w;
    this->renderPosition.h = __clipPos.h;

    SDL_RenderCopy(
        __render, this->texture, 
        &this->clipPosition, &this->renderPosition
    );
}

int EventsControl::keyCount = SDL_NUM_SCANCODES;

void EventsControl::events(void)
{
    while (SDL_PollEvent(&this->sdlEvents) != 0)
    {
        switch (this->sdlEvents.type)
        {
            case SDL_QUIT:
              this->quit = true;
              break;

            case SDL_KEYDOWN:
                this->eventsRecord.at(sdlEvents.key.keysym.scancode) = true;
                break;
            
            case SDL_KEYUP:
                this->eventsRecord.at(sdlEvents.key.keysym.scancode) = false;
                break;
            
            default:
                break;
        }
    }
}