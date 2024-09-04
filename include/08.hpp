#ifndef __08_H__
#define __08_H__

#include "SDL.h"
#include "SDL_image.h"

#include <string>
#include <exception>
#include <vector>
#include <MyLib/myLogerDef.h>

#define SPEED 2

const int SCREEN_HEIGHT = 800;
const int SCREEN_WIDTH  = 1000;

const int CIRCLE_DIAMETER = 143;

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
         * @brief 获取纹理剪辑后的宽。
        */
        int getRelativeWidth (void)  const { return this->clipPosition.w; }

        /**
         * @brief 获取纹理剪辑后的高。
        */
        int getRelativeHeight(void)  const { return this->clipPosition.w; }

        /**
         * @brief 获取纹理原始的宽。
        */
        int getAbsoluteWidth(void)  const { return this->renderPosition.w; }

        /**
         * @brief 获取纹理原始的高。
        */
        int getAbsoluteHeight(void) const { return this->renderPosition.h; }
        
        /**
         * @brief 加载一张图片的纹理信息。
         * 
         * @param __path         图片路径
         * @param __render       渲染器
        */
        bool loadFromFile(std::string __path, SDL_Renderer * __render);

        /**
         * @brief 指定纹理相对于屏幕的渲染平面坐标（x, y）和 裁剪范围，交由渲染器渲染。
        */
        void render(int __x, int __y, SDL_Rect __clipPos, SDL_Renderer * __render);

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

#endif // __08_H__