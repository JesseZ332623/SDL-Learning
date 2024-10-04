#ifndef __TIMING_H__
#define __TIMING_H__

#include "../../../SDLTemplateCode/include/SDLTemplate.hpp"
#include "SDL_image.h"

#define FPS 8

class CialloAnimation
{   
    public:
        struct TimePiece { Uint64 lastTime; Uint64 currentTime; };
    
    private:
        SystemInit::WindowSize  windowSize;
        SystemInit              sysInit;
        EventsControl           events;
        
        IMG_Animation *                  cialloGIF;
        std::vector<SDL_Texture *>       GIFTextures;

        TextureImage                     cialloFloat;
        SDL_Point                        cialloFloatRenderPos;
        bool                             ifcialloFloatPlay;
        int                              textureAlpha;

        SoundEffects            soundEffects;
        TimePiece               playTimePiece;  // 音效的播放由鼠标左键触发，需要延迟

        Uint64                  startTime;
        Uint64                  renderTime;

        SDL_Rect                renderRect;

        void loadGIFTexture(void);

        void mouseEventBehavior(void);

        public:
            CialloAnimation(void) : 
            windowSize({800, 600}), sysInit(windowSize, "Ciallo~"), 
            ifcialloFloatPlay(false), textureAlpha(0xFF),
            soundEffects(DEAULT_AUDIO_ATTRIBUTION) {}

            void init(void);    

            void load(void);

            void run(void);

            ~CialloAnimation();
};

void CialloAnimation::loadGIFTexture(void)
{
    this->GIFTextures.resize(this->cialloGIF->count);

    for (int index = 0; index < this->cialloGIF->count; ++index)
    {
        this->GIFTextures.at(index) = SDL_CreateTextureFromSurface(
                                        this->sysInit.getRenderer(), 
                                        (this->cialloGIF->frames)[index]
                                    );
        //SDL_SetTextureColorMod(this->GIFTextures.at(index), 0, 0xFF, 0xFF);
    }
}

void CialloAnimation::mouseEventBehavior(void)
{
    const EventsControl::MouseButtonMap & mouseButtonMap = this->events.getMouseState();
    const SDL_Point & mousePos                           = this->events.getMousePosition();

    SDL_Point   textureBoder = {
                        this->renderRect.x + this->cialloGIF->w,
                        this->renderRect.y + this->cialloGIF->h
                    };
    
    SDL_Point   textureRenderPos = {this->renderRect.x, this->renderRect.y};
    
/**
  * @brief 查询特定鼠标按键事件的标志位。
*/
#define CONDITIONS(MouseKeyCode) (                                  \
    (mouseButtonMap.find(MouseKeyCode) != mouseButtonMap.end()) &&  \
    (mouseButtonMap.find(MouseKeyCode)->second)                     \
)

/**
  * @brief 鼠标位置限制，
  *        确保鼠标位置在纹理大小范围内时才进行相应操作。
*/
#define MOUSEPOS_ASTRICT ((mousePos <= textureBoder) && (mousePos >= textureRenderPos))

    if (CONDITIONS(SDL_BUTTON_LEFT) && MOUSEPOS_ASTRICT)
    {
        if ((this->playTimePiece.currentTime - this->playTimePiece.lastTime) > 375)
        {
            this->soundEffects.play(-1, 0);
            this->playTimePiece.lastTime = this->playTimePiece.currentTime;
            this->ifcialloFloatPlay = true;
        }
    }
}

void CialloAnimation::init(void)
{
    this->sysInit.init();
    soundEffects.init(true);

    this->soundEffects.setVolume(5);

    this->startTime = SDL_GetTicks64();
    this->renderTime = STANDING_RENDER_TIME(this->startTime, FPS);
}

void CialloAnimation::load(void)
{
    this->soundEffects.load("../audio/effects/ciallo.wav");
    this->cialloGIF = IMG_LoadAnimation("../img/ciallo/ciallo.gif");

    this->loadGIFTexture();
    this->cialloFloat.load(
        "../img/ciallo/ciallo_frame_01.png", 
        {0xFF, 0xFF, 0xFF, 0xFF}, SDL_TRUE, this->sysInit.getRenderer()
    );
}

void CialloAnimation::run(void)
{
    int currentFrame = 0;
    this->renderRect = {
        this->windowSize.w / 2 - this->cialloGIF->w / 2, 
        this->windowSize.h / 2 - this->cialloGIF->h / 2, 
        this->cialloGIF->w, this->cialloGIF->h
    };

    this->cialloFloatRenderPos = {
        (this->windowSize.w / 2 - this->cialloGIF->w / 2),
        (this->windowSize.h / 2 - this->cialloGIF->h / 2) - 35
    };

#define BLACK_RENDERER_COLOR    SDL_SetRenderDrawColor(                             \
                                    this->sysInit.getRenderer(), 0, 0, 0, 0xFF      \
                            );                                                      \
                                    

#define WHITE_RENDERER_COLOR    SDL_SetRenderDrawColor(                             \
                                    this->sysInit.getRenderer(),                    \
                                    0xFF, 0xFF, 0xFF, 0xFF                          \
                            );                                                      \
                                                                                     

    while (!this->events.getRunstate())
    {
        this->events.recordEvents();

        SDL_RenderClear(this->sysInit.getRenderer());
        WHITE_RENDERER_COLOR

        this->playTimePiece.currentTime = SDL_GetTicks64();
        this->mouseEventBehavior();

#if true
        BLACK_RENDERER_COLOR
        SDL_RenderDrawRect(sysInit.getRenderer(), &renderRect);
        WHITE_RENDERER_COLOR
#endif
        
        SDL_RenderCopy(
            sysInit.getRenderer(), this->GIFTextures.at(currentFrame), 
            nullptr, &this->renderRect
        );

        if (this->ifcialloFloatPlay)
        {
            this->cialloFloat.render(
                cialloFloatRenderPos.x, cialloFloatRenderPos.y, 
                this->sysInit.getRenderer(), this->cialloFloat.defaultClip()
            );

            this->cialloFloat.render(
                cialloFloatRenderPos.x + 300, cialloFloatRenderPos.y, 
                this->sysInit.getRenderer(), this->cialloFloat.defaultClip()
            );

            this->cialloFloatRenderPos.y -= 15;
            this->cialloFloat.setAlpha((textureAlpha -= 17));

            if (cialloFloatRenderPos.y + this->cialloFloat.getRenderPosition().h < 0) {
                this->ifcialloFloatPlay = false;
                this->textureAlpha = 0xFF;
                this->cialloFloatRenderPos.y = (this->windowSize.h / 2 - this->cialloGIF->h / 2) - 35;
            }
        }

        SDL_RenderPresent(this->sysInit.getRenderer());

        if (currentFrame == this->cialloGIF->count - 1) { currentFrame = 0; }
        ++currentFrame;

        frameControl(this->startTime, this->renderTime);
    }
}

CialloAnimation::~CialloAnimation()
{
    IMG_FreeAnimation(this->cialloGIF);

    for (auto & texture : this->GIFTextures)
    {
        SDL_DestroyTexture(texture);
    }
}

#endif // __TIMING_H__