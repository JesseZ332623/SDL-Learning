/**
 * @file SDLTemplateCodeTest4.cpp
 * 
 * @brief 测试 GIFAnimation 类，渲染一个 GIF 动画。
 * 
 * @date 2024-10-04
 * 
 * @copyright Copyright (c) 2024
 * 
*/

#include "../../SDLTemplateCode/include/SDLTemplate.hpp"

#ifdef main
#undef main
#endif

class GIFRender
{
    private:
        SystemInit::WindowSize      windowSize;
        SystemInit                  sysInit;

        EventsControl               events;

        GIFAnimation                animation;
        int                         currentFrame;
        SDL_Rect                    renderInfo; 

        Uint64                      startTime;
        Uint64                      renderTime;

        void playGIFAnimation(int __frameIndex, SDL_Rect & __renderInfo);

        void frameAdjust(void);
    
    public:
        GIFRender(void) : 
        windowSize({600, 400}), sysInit(windowSize, "GIF Animation render"),
        currentFrame(0), startTime(0), renderTime(0) {}

        void init(void);

        void load(void);

        void run(void);
};

void GIFRender::playGIFAnimation(int __frameIndex, SDL_Rect & __renderInfo)
{
    SDL_RenderCopy(
        this->sysInit.getRenderer(), 
        this->animation.getAnimationTextures().at(__frameIndex),
        nullptr, &this->renderInfo
    );
}

void GIFRender::frameAdjust(void)
{
    this->currentFrame += 1;

    if (this->currentFrame > this->animation.getAnimationInfo()->count - 1) {
        this->currentFrame = 0;
    }
}

void GIFRender::init(void)
{
    this->sysInit.init();
    this->startTime = SDL_GetTicks64();
}

void GIFRender::load(void)
{
    this->animation.load("../img/ciallo/ciallo.gif", sysInit.getRenderer());
    
    this->renderInfo.w = this->animation.getAnimationInfo()->w;
    this->renderInfo.h = this->animation.getAnimationInfo()->h;

    this->renderTime = *(this->animation.getAnimationInfo()->delays);
}

void GIFRender::run(void)
{
    this->renderInfo.x = this->windowSize.w / 2 - this->animation.getAnimationInfo()->w / 2;
    this->renderInfo.y = this->windowSize.h / 2 - this->animation.getAnimationInfo()->h / 2;
    while (!this->events.getRunstate())
    {
        this->events.recordEvents();

        SDL_RenderClear(sysInit.getRenderer());
        SDL_SetRenderDrawColor(sysInit.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);

        this->playGIFAnimation(this->currentFrame, this->renderInfo);

        SDL_RenderPresent(sysInit.getRenderer());

        this->frameAdjust();

        frameControl(this->startTime, this->renderTime);
    }
}

int main(int argc, char const *argv[])
{
    system("cls");

    GIFRender GIFRender;

    GIFRender.init();
    GIFRender.load();
    GIFRender.run();

    return EXIT_SUCCESS;
}
