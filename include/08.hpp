#ifndef __08_H__
#define __08_H__

#include "SDL.h"
#include "SDL_image.h"

#include <string>
#include <exception>
#include <vector>
#include <ctime>
#include <cmath>
#include <pcg_basic.h>
#include <MyLib/myLogerDef.h>

#define BARRIER_COUNT 6  // 屏幕内障碍物的数量
#define BARRIER_SIZE  75  // 屏障的边长（像素）
#define BARRIER_COLOR {0xAF, 0xAD, 0xBF}
const int BARRIER_DIAGONAL = std::sqrt(std::pow(BARRIER_SIZE, 2) + std::pow(BARRIER_SIZE, 2));


#define SPEED 5         // 圆形的移速
#define FPS   120

const int SCREEN_HEIGHT = 1000;
const int SCREEN_WIDTH  = 1400;

const int CIRCLE_DIAMETER = 93;

/**
 * @brief 初始化类，该类封装了程序所有的初始化操作。 
*/
class SystemInit
{
    private:
        static std::string windowName;
        SDL_Window   * mainWindow;
        SDL_Renderer * render;

        void SDLMainInit(void);
        void createWindow(void);
        void createRenderer(void);
        void SDLImageInit(void);

    public:
        SystemInit(void) : mainWindow(nullptr), render(nullptr) {}

        /**
         * @brief 系统所有组件的初始化，按顺序分别是：
         *        1. SDL 主模块
         *        2. 主窗口
         *        3. 基于窗口的渲染器
         *        4. SDL image 模块
        */
        void init(void);

        /**
         * @brief 获取窗口类。
        */
        SDL_Window * getWindow()   const  { return this->mainWindow; }

        /**
         * @brief 获取渲染器类。
        */
        SDL_Renderer * getRenderer() const { return this->render; }

        /**
         * @brief 析构函数，销毁窗口与渲染器，
         *        以及所有的 SDL 模块。
        */
        ~SystemInit();
};

/**
 * @brief SDL_Texture 的简单封装。
*/
class Texture
{
    private:
        SDL_Texture * texture;      // SDL 纹理   

        SDL_Rect renderPosition;    // 相对于屏幕的渲染位置
        SDL_Rect clipPosition;      // 相对于整个纹理的渲染位置（剪辑渲染）

        /**
         * @brief 销毁纹理并重置所有成员数据。
        */
        void freeTexture(void);

        /**
         * @brief 设置纹理的裁剪范围。
        */
        void setClipPosition(SDL_Rect & __clipPos);

    public:
        Texture(void) : texture(nullptr), renderPosition({0, 0, 0, 0}), clipPosition({0, 0, 0, 0}) {}

        /**
         * @brief 获取该纹理的原始长宽，以及该纹理渲染在屏幕的位置。
        */
        SDL_Rect getRenderPosition(void) const { return this->renderPosition; }

        /**
         * @brief 获取该纹理的裁剪位置和长宽。
        */
        SDL_Rect getClipPosition(void) const { return this->clipPosition; }
        
        /**
         * @brief 加载一张图片的纹理信息。
         * 
         * @param __path         图片路径
         * @param __render       渲染器
        */
        bool loadFromFile(std::string __path, SDL_Renderer * __render);

        /**
         * @brief 加载一个指定了宽高的矩形纹理。
        */
        bool loadRectangle(int __w, int __h, SDL_Renderer * __render);

        /**
         * @brief 指定纹理相对于屏幕的渲染平面坐标（x, y）和 裁剪范围，并交由渲染器渲染。
         * 
         * @brief - 该方法针对图片的渲染
        */
        void render(int __x, int __y, SDL_Rect & __clipPos, SDL_Renderer * __render);

        /**
         * @brief 指定纹理相对于屏幕的渲染平面坐标（x, y）和颜色（RGB），并交由渲染器渲染。
         * 
         * @brief - 该方法针对矩形的渲染
        */
        void render(int __x, int __y, SDL_Color __color, SDL_Renderer * __render);

        /**
         * @brief 销毁该纹理，并重置所有成员函数。
        */
        ~Texture() { this->freeTexture(); }
};

/**
 * @brief SDL 事件控制类
*/
class EventsControl
{
    public:
        static int keyCount;        // SDL 的总按键数量

    private:
        SDL_Event  sdlEvents;

        bool quit;
        std::vector<bool> eventsRecord;

    public:
        EventsControl(void) : sdlEvents(), quit(false), eventsRecord(keyCount) {}

        void events(void);

        const bool getRunState(void) const { return this->quit; }

        const std::vector<bool>
        getEventsRecord(void) const { return this->eventsRecord; }
};

/**
 * @brief 基于 PCG 的随机数生成器 
*/
class RandomGenerater
{
    private:
        pcg32_random_t randomInstance;

    public:
        /**
         * @brief 初始化随机数实例，并设置随机数种子。
        */
        RandomGenerater(void) : randomInstance({0}) {
            Uint64 currentTime = static_cast<Uint64>(std::time(nullptr));
            pcg32_srandom_r(&this->randomInstance, currentTime, currentTime + 1);
        }

        /**
         * @brief 生成一定范围（__min <= randomNum < __max）的随机数。
        */
        Uint32 getRand(Uint32 __min, Uint32 __max) { 
            return pcg32_boundedrand_r(&this->randomInstance, __max - __min) + __min; 
        }
};

#endif // __08_H__