#include "./ciallo~.h"

#include "../../../SDLTemplateCode/include/pointCompare.hpp"

//#include <thread>

void Danmaku::load(std::string __path, int __speed, int __posY, SDL_Color __color, SDL_Renderer * __render)
{
    this->cialloImage.load(
        __path, {0xFF, 0xFF, 0xFF, 0xFF}, SDL_TRUE, __render
    );

    this->moveSpeed = __speed;
    this->color     = {__color.r, __color.g, __color.b};

    if (__posY < this->cialloImage.getRenderPosition().h) 
    {
        __posY += (this->cialloImage.getRenderPosition().h - __posY);
    }
    else if (__posY > SCREEN_HEIGHT - this->cialloImage.getRenderPosition().h)
    {
        __posY -= (this->cialloImage.getRenderPosition().h - __posY);
    }

    this->renderPos = {SCREEN_WIDTH, __posY};
}

void Danmaku::render(SDL_Renderer * __render)
{
    this->cialloImage.setColor(this->color);
    this->cialloImage.render(
        this->renderPos.x, this->renderPos.y, 
        __render, this->cialloImage.defaultClip()
    );
}

void DanmakuManager::add(SDL_Renderer * __render)
{
    Danmaku * tempDanmaku = new Danmaku();

    tempDanmaku->load(
        "../img/ciallo/ciallo_frame_01.png", 
        speedDistrbution(this->randomEngine), posYDistribution(this->randomEngine),
        {colorDistribution(this->randomEngine), colorDistribution(this->randomEngine), colorDistribution(this->randomEngine)},
        __render
    );

    danmakus.push_back(tempDanmaku);
}

void DanmakuManager::update(void)
{
    for (auto it = this->danmakus.begin(); it != this->danmakus.end();)
    {
        (*it)->update();

        if ((*it)->isOffsetScreem()) {
            delete (*it);
            it = this->danmakus.erase(it);
        }
        else {
            ++it;
        }
    }
}

void DanmakuManager::render(SDL_Renderer * __render)
{
    for (auto & danmaku : this->danmakus)
    {
        danmaku->render(__render);
    }
}

DanmakuManager::~DanmakuManager()
{
    for (auto & danmaku : this->danmakus)
    {
        delete danmaku;
    }
}

void CialloAnimation::playGIFAnimation(void)
{
    SDL_RenderCopy(
        this->sysInit.getRenderer(), 
        this->cialloAnimation.getAnimationTextures().at(currentFrame),
        nullptr, &this->animationRenderRect
    );
}

void CialloAnimation::playCialloImage(void)
{
    if (this->ifCialloImageRender)
    {
        this->cialloImage.render(
            this->cialloImageRenderPos.x, this->cialloImageRenderPos.y,
            this->sysInit.getRenderer(), this->cialloImage.defaultClip()
        );

        this->cialloImageRenderPos.y -= 1;
        this->cialloImage.setAlpha(this->cialloImageAlpha -= 1);

        if ((this->cialloImageRenderPos.y + this->cialloImage.getRenderPosition().h <= 0) || (this->cialloImageAlpha <= 0))
        {
            this->cialloImageAlpha       = 0xFF;
            this->cialloImageRenderPos.y = this->animationRenderRect.y;
            this->ifCialloImageRender    = false;
        }
    }
}

void CialloAnimation::GIFAnimationFrameControl(void)
{
    this->currentFrame += 1;

    if (this->currentFrame > this->cialloAnimation.getAnimationInfo()->count - 1)
    {
        this->currentFrame = 0;
    }
}

void CialloAnimation::mouseEventBehavior(void)
{
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
#define MOUSEPOS_ASTRACT ((mousePos <= textureBorderRight) && (mousePos >= textureBorderLeft))

    const SDL_Point & mousePos                           = this->events.getMousePosition();
    const EventsControl::MouseButtonMap & mouseButtonMap = this->events.getMouseState();

/**
 * @brief 本来想把左键生效范围限制在 cialloAnimation 帧纹理范围内，
 *        现在看来直接跟随鼠标位置更好。
*/
#if false
    static SDL_Point textureBorderLeft  = {this->animationRenderRect.x, this->animationRenderRect.y};
    static SDL_Point textureBorderRight = {
        this->animationRenderRect.x + this->cialloAnimation.getAnimationInfo()->w,
        this->animationRenderRect.y + this->cialloAnimation.getAnimationInfo()->h,
    };
#endif

    if (CONDITIONS(SDL_BUTTON_LEFT))
    {
        if ((this->playTimePiece.currentTime - this->playTimePiece.lastTime) > 450)
        {
            this->soundEffects.play(-1, 0);
            this->playTimePiece.lastTime = this->playTimePiece.currentTime;
            this->cialloImageRenderPos.x = mousePos.x;
            this->cialloImageRenderPos.y = mousePos.y;
            this->cialloImageAlpha       = 0xFF;
            this->cialloImage.setColor({RANDOM_COLOR_VALUE, RANDOM_COLOR_VALUE, RANDOM_COLOR_VALUE});
            this->clickCount++;
            this->ifCialloImageRender    = true;
        }
    }
# if false
    if (this->events.getKeyboardState().size() != 0)
    {
        EventsControl::KeyState & spaceState = keyCodeMap.at(SDL_SCANCODE_SPACE);

       if (
            spaceState.pressed && !spaceState.processed && 
            (this->playTimePiece.currentTime - this->playTimePiece.lastTime) > 175
        ) {
            this->playTimePiece.lastTime = this->playTimePiece.currentTime;
            this->clickCount             = 0;
        }

        spaceState.processed = true;
    }
#endif
}

void CialloAnimation::keyboradEventsBehavior(void)
{
    //EventsControl::KeyCodeMap & keyCodeMap     = this->events.getKeyboardState();

    if (this->events.getKeyboardState().size() == 0) { return; }

    Uint64 currentTime = SDL_GetTicks64();

    for (auto & [keyCode, keyState] : this->events.getKeyboardState())
    {
        if (keyState.pressed && !keyState.processed && (currentTime - keyState.lastKeyPressTime) > 80)
        {
            switch (keyCode)
            {
                case SDL_SCANCODE_SPACE:
                    clickCount         = 0;
                    keyState.processed = true;
                    break;
                default:
                    break;
            }
        }   
    }
}

void CialloAnimation::init(void)
{
    this->sysInit.init();

    this->soundEffects.init(true);
    this->soundEffects.setVolume(25);

    this->startTime  = SDL_GetTicks64();
    this->renderTime = STANDING_RENDER_TIME(this->startTime, FPS);
}

void CialloAnimation::load(void)
{
    this->soundEffects.load("../audio/effects/ciallo.wav");
    this->cialloAnimation.load("../img/ciallo/ciallo.gif", this->sysInit.getRenderer());
    this->cialloImage.load(
        "../img/ciallo/ciallo_frame_01.png", {0xFF, 0xFF, 0xFF, 0xFF},
        SDL_TRUE, this->sysInit.getRenderer()
    );
    this->clickCountShown.open("../fonts/msyhbd.ttc", 50);

    this->animationRenderRect.w = this->cialloAnimation.getAnimationInfo()->w;
    this->animationRenderRect.h = this->cialloAnimation.getAnimationInfo()->h;
    //this->renderTime            = *(this->cialloAnimation.getAnimationInfo()->delays);
}

void CialloAnimation::run(void)
{
#define BLACK_RENDERER_COLOR    \
    SDL_SetRenderDrawColor(this->sysInit.getRenderer(), 0, 0, 0, 0xFF)                                                      \
                                        
#define WHITE_RENDERER_COLOR     \
    SDL_SetRenderDrawColor(this->sysInit.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF)                                                    

    this->animationRenderRect.x = this->windowSize.w / 2 - this->cialloAnimation.getAnimationInfo()->w / 2;
    this->animationRenderRect.y = this->windowSize.h / 2 - this->cialloAnimation.getAnimationInfo()->h / 2; 

    std::uniform_int_distribution<> danmakuGenerate(0, 2);                                                                                

    while (!this->events.getRunstate())
    {
        this->events.recordEvents();

        SDL_RenderClear(this->sysInit.getRenderer());
        WHITE_RENDERER_COLOR;

        this->playTimePiece.currentTime = SDL_GetTicks64();
        this->mouseEventBehavior();

        this->keyboradEventsBehavior();

        BLACK_RENDERER_COLOR;
        SDL_RenderDrawRect(sysInit.getRenderer(), &this->animationRenderRect);
        WHITE_RENDERER_COLOR;

        this->danmakuManager.update();
        this->danmakuManager.render(this->sysInit.getRenderer());

        this->playGIFAnimation();
        this->playCialloImage();

        this->clickCountShown.load(
            std::to_string(this->clickCount), 
            {0, 0, 0, 0xFF}, sysInit.getRenderer()
        );

        this->clickCountShown.render(10, 5, sysInit.getRenderer());

        if (danmakuGenerate(this->danmakuManager.getRandomEngine()) == 0)
        {
            this->danmakuManager.add(this->sysInit.getRenderer());
        }

        SDL_RenderPresent(this->sysInit.getRenderer());

        this->GIFAnimationFrameControl();
        
        frameControl(this->startTime, this->renderTime);
    }
}