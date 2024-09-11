#include "SDL.h"
#include "SDL_image.h"

#include <string>

/**
 * @brief 初始化类，该类封装了程序所有的初始化操作。 
*/
class SystemInit
{
    public:
        struct WindowSize {int w; int h;};

    private:

        SDL_Window   * mainWindow;
        WindowSize     windowSize;
        std::string    windowName;

        SDL_Renderer * render;

        void SDLMainInit(void);
        void createWindow(const WindowSize & __windowSize);
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
        void init(WindowSize __windowSize, std::string __windowName);

        /**
         * @brief 获取窗口类。
        */
        SDL_Window * getWindow(void)     const { return this->mainWindow; }

        /**
         * @brief 获取窗口名。
        */
        std::string getWindowName(void)  const { return this->windowName; };

        /**
         * @brief 获取窗口大小。
        */
        WindowSize getWindowSize(void) const { return this->windowSize; }

        /**
         * @brief 获取渲染器类。
        */
        SDL_Renderer * getRenderer(void) const { return this->render; }

        /**
         * @brief 析构函数，销毁窗口与渲染器，
         *        以及所有的 SDL 资源。
        */
        ~SystemInit();
};