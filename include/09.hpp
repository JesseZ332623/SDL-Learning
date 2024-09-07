#ifndef __09_H__
#define __09_H__

#include "SDL.h"
#include "SDL_image.h"

#include <string>
#include <exception>
#include <vector>
#include <chrono>
#include <random>
#include <cstring>
#include <MyLib/myLogerDef.h>

#define FPS   120       // 程序需要维持的帧数
#define ADJUST_VALUE 1

#define SDL_INIT_FLAGS (SDL_INIT_VIDEO | SDL_INIT_AUDIO)
#define IMG_INIT_FLAGS (IMG_INIT_PNG | IMG_INIT_JPG)

const int SCREEN_HEIGHT = 480;
const int SCREEN_WIDTH  = 640;

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

        /**
         * @brief 销毁纹理并重置所有成员数据。
        */
        void freeTexture(void);

        /**
         * @brief 设置纹理的裁剪范围。
        */
        void setClipPosition(SDL_Rect & __clipPos);

    public:
        Texture(void) : texture(nullptr), renderPosition({0}) {}

        /**
         * @brief 获取该纹理的原始长宽，以及该纹理渲染在屏幕的位置。
        */
        SDL_Rect getRenderPosition(void) const { return this->renderPosition; }

        /**
         * @brief 加载一张图片的纹理信息。
         * 
         * @param __path         图片路径
         * @param __render       渲染器
        */
        bool loadFromFile(std::string __path, SDL_Renderer * __render);

        /**
         * @brief 设置纹理的颜色值（RGBA）
        */
        void setColor(SDL_Color __color);

        /**
         * @brief 指定纹理相对于屏幕的渲染平面坐标（x, y）和 裁剪范围，并交由渲染器渲染。
         * 
         * @brief - 该方法针对图片的渲染
        */
        void render(int __x, int __y, SDL_Renderer * __render);

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
 * @brief 基于 <random> 的随机数生成器 
*/
class RandomGenerater
{
    private:
        std::random_device device;
        std::mt19937_64    randomInstance;

    public:
        RandomGenerater(void) 
        {
             // 获取当前时间作为额外的熵
            auto now = std::chrono::high_resolution_clock::now();
            auto time_since_epoch = now.time_since_epoch();
            auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(time_since_epoch).count();

            // 将随机设备的值与当前时间结合，增加随机性
            unsigned long long seed = device() + static_cast<unsigned long long>(nanoseconds);
            randomInstance.seed(seed);
        }
        
        /**
         * @brief 生成 __min <= randNum < __max 的随机数
        */
        int getRandNum(int __min, int __max);
};

#endif // __09_H__