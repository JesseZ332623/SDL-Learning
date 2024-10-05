#ifndef __TIMING_H__
#define __TIMING_H__

#include "../../../SDLTemplateCode/include/SDLTemplate.hpp"

#include <random>

#define DANMAKU_AMOUNT 55       // 弹幕总数
#define SCREEN_WIDTH   1800
#define SCREEN_HEIGHT  1000

#define FPS            165

/**
 * @brief 颜色值随机选取。
*/
#define RANDOM_COLOR_VALUE (static_cast<Uint8>(std::rand() % 254))

class Danmaku
{
    private:
        TextureImage            cialloImage;

        SDL_Color               color;
        Uint32                  moveSpeed;
        SDL_Point               renderPos;
    
    public:
        Danmaku(void) : 
        color(), moveSpeed(0), renderPos() {}

        bool isOffsetScreem(void) const {
            return this->renderPos.x < -this->cialloImage.getRenderPosition().w;
        }

        void update(void) {
            this->renderPos.x -= this->moveSpeed;
        }

        void load(
            std::string __path, 
            int __speed, int __posY, SDL_Color __color,
            SDL_Renderer * __render
        );

        void render(SDL_Renderer * __render);
};

class DanmakuManager
{
    private:
        std::vector<Danmaku *> danmakus;

        std::random_device      randomDevice;
        std::mt19937_64         randomEngine;

        std::uniform_int_distribution<int>   speedDistrbution;
        std::uniform_int_distribution<Uint8> colorDistribution;
        std::uniform_int_distribution<int>   posYDistribution;

    public:
        DanmakuManager(void) : randomEngine(randomDevice()),
        speedDistrbution(3, 10), colorDistribution(1, 255), posYDistribution(0, SCREEN_HEIGHT) {}

        std::size_t getDanmakusAmount(void) const {
            return this->danmakus.size();
        }

        std::mt19937_64 & getRandomEngine(void) {
            return this->randomEngine;
        }

        void add(SDL_Renderer * __render);

        void update(void);

        void render(SDL_Renderer * __render);

        ~DanmakuManager();
};

class CialloAnimation
{   
    public:
        struct TimePiece { Uint64 lastTime; Uint64 currentTime; };
    
    private:
        SystemInit::WindowSize  windowSize;
        SystemInit              sysInit;
        EventsControl           events;

        GIFAnimation            cialloAnimation;        // Ciallo~ GIF 动画
        SDL_Rect                animationRenderRect;    // 动画渲染信息
        Uint8                   currentFrame;           // 当前渲染应该渲染的帧计数

        /**
         * @brief 该纹理的渲染由鼠标事件触发，渲染位置随鼠标移动。
         */
        TextureImage            cialloImage;
        SDL_Point               cialloImageRenderPos;
        bool                    ifCialloImageRender;
        int                     cialloImageAlpha;

        DanmakuManager          danmakuManager;
        
        FontsTexture            clickCountShown;
        Uint32                  clickCount;


        /**
         * @brief 音效的播放由鼠标左键触发，
         *        可以设置一个计时器 playTimePiece 进行延迟避免频繁触发。
        */
        SoundEffects            soundEffects;
        TimePiece               playTimePiece;

        Uint64                  startTime;
        Uint64                  renderTime;

        /**
         * @brief GIF 动画的播放。 
        */
        void playGIFAnimation(void);

        /**
         * @brief Ciallo~ 图片纹理经鼠标事件触发后的播放逻辑。
        */
        void playCialloImage(void);

        /**
         * @brief GIF 动画的播放控制。
        */
        void GIFAnimationFrameControl(void);

        /**
         * @brief 对于鼠标按键按下时要做的操作。 
        */
        void mouseEventBehavior(void);

        /**
         * @brief 对于键盘按键按下时要做的操作。 
        */
        void keyboradEventsBehavior(void);

        public:
            CialloAnimation(void) : 
            windowSize({SCREEN_WIDTH, SCREEN_HEIGHT}), sysInit(windowSize, "Ciallo~"), 
            currentFrame(0), ifCialloImageRender(false), cialloImageAlpha(0xFF),
            clickCount(0), soundEffects(DEAULT_AUDIO_ATTRIBUTION) {}

            void init(void);    

            void load(void);

            void run(void);
};

#endif // __TIMING_H__