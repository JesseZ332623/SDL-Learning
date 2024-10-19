#include "../../SDLTemplateCode/include/SDLTemplate.hpp"

#define FPS 30

class Timer
{
    public:
      struct TimePiece {Uint64 last; Uint64 current;};  
    private:
        SystemInit::WindowSize  windowSize;
        SystemInit              sysInit;

        EventsControl           events;

        FontsTexture            timer;
        Uint64                  timeSeconds;
        SDL_Point               timerRenderPos;
        TimePiece               updatetimePiece;
        int                     timeMinusVal;
        bool                    isTimerLoad;

        SoundEffects            timeOver;
        bool                    timeOverPlayState;
        TimePiece               timeOverPlayTimePiece;

        SoundEffects            tickA;
        SoundEffects            tickB;
        Uint8                   tickPlayOption;

        Uint64                  startTick;
        Uint64                  renderTick;

        Timer(int) : windowSize({400, 225}), sysInit(windowSize, "Timer"), 
        timeMinusVal(1), timeOver(DEAULT_AUDIO_ATTRIBUTION), timeOverPlayState(true),
        tickA(DEAULT_AUDIO_ATTRIBUTION), tickB(DEAULT_AUDIO_ATTRIBUTION), tickPlayOption(true) {}

        void getCenterRenderPos(void);

        void windowResizeHandle(void);

        void keyEventsHandle(void);

        void setTimeSeconds(std::string & __initTime);

        std::string getTimeString(void);

        bool isOver(void) const { return (this->timeSeconds == 0); }

        void recordTime(void);

    public:
        Timer(void) : Timer(0) {}

        void init(void);

        void load(std::string __initTime);

        void run(void);
};

void Timer::getCenterRenderPos(void)
{
    timerRenderPos.x = this->windowSize.w / 2 - this->timer.getRenderPosition().w / 2;
    timerRenderPos.y = this->windowSize.h / 2 - this->timer.getRenderPosition().h / 2;
}

void Timer::windowResizeHandle(void)
{
    if (this->events.getWindowSizeState().ifResize) 
    {
        this->windowSize.w = this->events.getWindowSizeState().width;
        this->windowSize.h = this->events.getWindowSizeState().height;

        this->getCenterRenderPos();
    }
}

void Timer::keyEventsHandle(void)
{
    if (this->events.getKeyboardState().size() == 0) { return; }

    Uint64 currentTime = SDL_GetTicks64();

    for (auto & [keyCode, keyState] : this->events.getKeyboardState())
    {
        if (keyState.pressed && !keyState.processed && (currentTime - keyState.lastKeyPressTime) > 45)
        {
            switch (keyCode)
            {
                case SDL_SCANCODE_P:
                    //printf("Pause\n");
                    this->timeMinusVal   = 0;
                    this->tickA.stop(-1);
                    this->tickB.stop(-1);
                    this->tickPlayOption = -1;
                    keyState.processed   = true;
                    break;
                
                case SDL_SCANCODE_R:
                    //printf("Restart\n");
                    this->timeMinusVal   = 1;
                    this->tickPlayOption = 1;
                    keyState.processed   = true;
                default:
                    break;
            }
        }
    }
}

void Timer::setTimeSeconds(std::string & __initTime)
{
    using std::stoi;
    using namespace fmt;

    int hours = 0, minuts = 0, seconds = 0;

    try 
    {
        hours   = stoi(__initTime.substr(0, 2));
        minuts  = stoi(__initTime.substr(5, 6));
        seconds = stoi(__initTime.substr(10, 11));
    }
    catch (const std::invalid_argument & __excpt) 
    {
       print(
            fg(terminal_color::red) | emphasis::bold, 
            "Timer::setTimeSeconds(): {}.\n", __excpt.what()
        );
    }
    catch (const std::out_of_range & __excpt)
    {
        print(
            fg(terminal_color::red) | emphasis::bold, 
            "Timer::setTimeSeconds(): {}.\n", __excpt.what()
        );
    }

    this->timeSeconds = static_cast<Uint64>(hours * 60 * 60 + minuts * 60 + seconds);
}

std::string Timer::getTimeString(void)
{
    int hours   = this->timeSeconds / 60 / 60;
    int minuts  = this->timeSeconds / 60 % 60;
    int seconds = this->timeSeconds % 60 % 60;

#define VALUE_CHECK(__val) ((__val < 10) ? '0' + std::to_string(__val) : std::to_string(__val))
    
    return {
        VALUE_CHECK(hours) + " : " + VALUE_CHECK(minuts) + " : " + VALUE_CHECK(seconds)
    };
}

void Timer::recordTime(void)
{
    this->updatetimePiece.current       = SDL_GetTicks64();
    this->timeOverPlayTimePiece.current = SDL_GetTicks64();

    if ((this->updatetimePiece.current - this->updatetimePiece.last) >= 1000) 
    {
        if (this->timeSeconds > 0) 
        {
            this->timeSeconds -= this->timeMinusVal;

            if (this->tickPlayOption == 1) 
            {
                this->tickA.play(-1, 0);
                this->tickPlayOption = false;
            }
            else if (this->tickPlayOption == 0)
            {
                this->tickB.play(-1, 0);
                this->tickPlayOption = true;
            }
        }

        this->isTimerLoad = true;

        this->updatetimePiece.last = this->updatetimePiece.current;
    }

    if (
        (timeOverPlayTimePiece.current - timeOverPlayTimePiece.last) >= 1000 &&
        this->isOver() && this->timeOverPlayState
    ) {
        this->timeOver.play(-1, 0);
        this->isTimerLoad = false;

        this->timeOverPlayTimePiece.last = this->timeOverPlayTimePiece.current;
    }
}

void Timer::init(void)
{
    this->sysInit.init();
    this->timeOver.init(true);

    this->startTick  = SDL_GetTicks64();
    this->renderTick = STANDING_RENDER_TIME(this->startTick, FPS);
}

void Timer::load(std::string __initTime)
{
    this->timer.open("../fonts/Consolas-Regular.ttf", 45);
    this->timer.load(__initTime, {0x00, 0xFF, 0x00, 0xFF}, this->sysInit.getRenderer());
    this->setTimeSeconds(__initTime);

    this->timeOver.load("../audio/effects/TimeOver.wav");

    this->tickA.load("../audio/effects/Tick_01.wav");
    this->tickB.load("../audio/effects/Tick_02.wav");
}

void Timer::run(void)
{
    this->getCenterRenderPos();
    this->updatetimePiece.last       = SDL_GetTicks64();
    this->timeOverPlayTimePiece.last = SDL_GetTicks64();

    this->sysInit.setNewWindowName("On The Clock");

    while (!this->events.getRunstate())
    {
        this->events.recordEvents();

        this->windowResizeHandle();
        this->recordTime();
        this->keyEventsHandle();

        if (this->isOver()) {
            this->sysInit.setNewWindowName("Time Over!");
        }

        if (this->isTimerLoad) 
        {
            SDL_SetRenderDrawColor(this->sysInit.getRenderer(), 0, 0, 0, 0xFF);
            this->timer.load(
                this->getTimeString(), 
                {0x00, 0xFF, 0x00, 0xFF}, this->sysInit.getRenderer()
            );
        }
        else {
            SDL_SetRenderDrawColor(this->sysInit.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
            this->timer.load(
                this->getTimeString(), 
                {0xFF, 0x00, 0x00, 0xFF}, this->sysInit.getRenderer()
            );
        }

        this->timer.render(
            this->timerRenderPos.x, this->timerRenderPos.y, 
            this->sysInit.getRenderer()
        );

        SDL_RenderPresent(this->sysInit.getRenderer());
        SDL_RenderClear(this->sysInit.getRenderer());

        frameControl(this->startTick, this->renderTick);
    }
}