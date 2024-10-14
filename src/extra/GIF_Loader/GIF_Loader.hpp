#include "../../../SDLTemplateCode/include/SDLTemplate.hpp"

#define FPS 165

class GIFLoader
{
    private:
        SystemInit::WindowSize  windowSize;
        SystemInit              sysInit;

        EventsControl           events;

        GIFAnimation            animation;

        Uint64                  startTime;
        Uint64                  renderTime;

        SDL_Point               centerRenderPos;                    

        void windowResizeBehavior(void);

        void keyPressBehaivor(void);

    public:
        GIFLoader(void) : windowSize({1, 1}), sysInit(windowSize, "GIF Loader"), centerRenderPos({0, 0}) {}

        void init(void);

        void load(std::string __path);

        void run(void);
};

void GIFLoader::windowResizeBehavior(void)
{
    if (this->events.getWindowSizeState().ifResize) 
    {
        this->windowSize.w = this->events.getWindowSizeState().width;
        this->windowSize.h = this->events.getWindowSizeState().height;

        this->centerRenderPos = {
            this->windowSize.w / 2 - this->animation.getAnimationInfo()->w / 2,
            this->windowSize.h / 2 - this->animation.getAnimationInfo()->h / 2
        };
    }
}

void GIFLoader::keyPressBehaivor(void)
{
    if (this->events.getKeyboardState().size() == 0) { return; }

    Uint64 currentTime = SDL_GetTicks64();

    for (auto & [keyCode, keyState] : this->events.getKeyboardState())
    {
        if (keyState.pressed && !keyState.processed && (currentTime - keyState.lastKeyPressTime) > 45)
        {
            switch (keyCode)
            {
                case SDL_SCANCODE_SPACE:
                    
                    if (this->animation.playState()) {
                        this->animation.pause();
                        this->sysInit.setNewWindowName("Pause");
                    }
                    else {
                        this->animation.setFrameSkip(1);
                        this->animation.replay();
                        this->sysInit.setNewWindowName("Playing: " + this->animation.getAnimationPath());
                    }

                    keyState.processed = true;

                    break;
                
                case SDL_SCANCODE_R:
                    this->animation.reset();
                    this->sysInit.setNewWindowName("Playing: " + this->animation.getAnimationPath());
                    keyState.processed = true;
                    break;

                default:
                    break;
            }
        }   
    }
}

void GIFLoader::init(void)
{
    this->sysInit.init();
}

void GIFLoader::load(std::string __path)
{
    try {
        this->animation.load(__path, this->sysInit.getRenderer());
    }
    catch (const std::runtime_error & __except)
    {
        fmt::print(fmt::fg(fmt::terminal_color::red), "{}", __except.what());

        this->animation.~GIFAnimation();
        this->sysInit.~SystemInit();

        exit(EXIT_FAILURE);
    }

    this->windowSize = {this->animation.getAnimationInfo()->w, this->animation.getAnimationInfo()->h};
    this->sysInit.resetWindowSize({this->windowSize.w, this->windowSize.h});

    this->animation.showAnimationInfo();
}

void GIFLoader::run(void)
{
    this->sysInit.setNewWindowName("Playing: " + this->animation.getAnimationPath());
    
    while (!this->events.getRunstate())
    {
        this->events.recordEvents();

        SDL_RenderClear(this->sysInit.getRenderer());
        SDL_SetRenderDrawColor(this->sysInit.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);

        this->animation.play(this->centerRenderPos.x, this->centerRenderPos.y, this->sysInit.getRenderer());

        SDL_RenderPresent(this->sysInit.getRenderer());

        this->windowResizeBehavior();
        this->keyPressBehaivor();

        SDL_Delay(*this->animation.getAnimationInfo()->delays);
    }
}