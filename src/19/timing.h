#ifndef __TIMING_H__
#define __TIMING_H__

#include "../../SDLTemplateCode/include/SDLTemplate.hpp"

#define FPS 165

class TimingShown
{
    public:
        //struct TimePeice { Uint64 lastTime; Uint64 currentTime;};
    private:
        SystemInit::WindowSize  windowSize;
        SystemInit              sysInit;
        EventsControl           events;
        //TimePeice               keyDelay;
        
        FontsTexture            timerShown;         // 显示当前时间的字符串纹理
        SoundEffects            soundEffects;       // 每当切换的时候，播放此音效
        Uint64                  timeShownNumber;

        Uint64                  startTime;
        Uint64                  renderTime;

        SDL_Point               timeShonPos;

        void keyEventsBehavior(void);

        public:
            TimingShown(void) : 
            windowSize({600, 400}), sysInit(windowSize, "19. Timing Shown"),
            soundEffects(DEAULT_AUDIO_ATTRIBUTION) {}

            void init(void);    

            void load(void);

            void run(void);
};

void TimingShown::keyEventsBehavior(void)
{
    if (!this->events.getKeyboardState().size()) { return; }

    Uint64 currentTime = SDL_GetTicks64();

    for (auto & [scanCode, keyState] : this->events.getKeyboardState())
    {
        if (keyState.pressed && !keyState.processed && (currentTime - keyState.lastKeyPressTime) > 80)
        {
            this->soundEffects.play(0, 0);
            this->timeShownNumber = SDL_GetTicks64();

            keyState.processed = true;
        }

        if (keyState.released) { } 
    }
}

void TimingShown::init(void)
{
    this->sysInit.init();
    this->soundEffects.init(true);

    this->startTime  = SDL_GetTicks64();
    this->renderTime = STANDING_RENDER_TIME(startTime, FPS);
}

void TimingShown::load(void)
{
    this->timerShown.open("../fonts/Consolas-Regular.ttf", 30);
    this->soundEffects.load("../audio/effects/Harp Gliss Up.wav");
}

void TimingShown::run(void)
{
    while (!this->events.getRunstate())
    {
        this->events.recordEvents();

        timeShonPos = {
            this->windowSize.w / 2 - this->timerShown.getRenderPosition().w / 2,
            this->windowSize.h / 2 - this->timerShown.getRenderPosition().h / 2
        };

        this->timerShown.load(
            "Current tick: [" + std::to_string((static_cast<float>(SDL_GetTicks64() - this->timeShownNumber) / 1000.0000)) + ']', 
            {0, 0, 0, 0xFF}, this->sysInit.getRenderer()
        );

        SDL_RenderClear(this->sysInit.getRenderer());
        SDL_SetRenderDrawColor(
            this->sysInit.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF
        );

        this->timerShown.render(timeShonPos.x, timeShonPos.y, this->sysInit.getRenderer());

        SDL_RenderPresent(this->sysInit.getRenderer());

        this->keyEventsBehavior();

        frameControl(this->startTime, this->renderTime);
    }
}

#endif // __TIMING_H__