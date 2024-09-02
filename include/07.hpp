#ifndef __07_H__
#define __07_H__

#include "SDL.h"
#include "SDL_image.h"

#include <string>
#include <MyLib/myLogerDef.h>

/**
 * @brief 初始化类，该类封装了程序所有的初始化操作。 
*/
class SystemInit
{
    private:
        SDL_Window   * mainWindow;
        SDL_Renderer * render;

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

class Texture
{
    private:
        int width;
        int height;
        SDL_Texture * texture;
        SDL_Rect renderPosition;

        /**
         * @brief 销毁纹理并重置所有成员数据。
        */
        void freeTexture(void);
    public:
        Texture(void) : width(0), height(0), texture(nullptr), renderPosition({0, 0, 0, 0}) {}

        int getWidth(void)  const { return this->width; }
        int getHeight(void) const { return this->height; }
        
        bool loadFromFile(std::string __path, SDL_Renderer * __render);

        void render(int __x, int __y, SDL_Renderer * __render);

        ~Texture() { this->freeTexture(); }
};

class EventsControl
{
    private:
        struct EventsInfo { bool quit; };

        SDL_Event sdlEvents;
        EventsInfo info;

    public:
        EventsControl(void) : sdlEvents(), info({false}) {}

        void events(void);

        const EventsInfo getInfoRecord(void) const { return this->info; }
};

#endif // __07_H__