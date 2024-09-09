#include "../../include/09-10.hpp"

/**
 * @brief 维持帧数在一个固定值。
 * 
 * @param __startFrameTick      // 某一帧开始渲染前的时间刻
 * @param __frameRenderTime     // 维持一定帧数渲染一帧所需要的标准时间
*/
void frameControl(Uint64 & __startFrameTick, const Uint64 __frameRenderTime);

/**
 * @brief 通过键盘事件来调整图片的颜色和透明度
*/
void modulation(const std::vector<bool> & __eventRecord, SDL_Color & __color);

int WinMain(int argc, char const *argv[])
{
    using namespace MyLib::MyLoger;

    system("cls");

    SystemInit      sysInit;            // 初始化所有 SDL 组件类
    RandomGenerater randomGenerater;    // 随机数生成器    

    Texture       colorRectangel;              
    Texture       girl;

    EventsControl eventsControl;        // 事件控制类

    sysInit.init(); // 初始化所有 SDL 组件

    Uint64 startFrameTick        = SDL_GetTicks64();        // SDL 所有组件完成初始化后的时间刻值。
    Uint64 frameRenderTime       = startFrameTick / FPS;    // 维持一定帧数渲染一帧所需要的标准时间

    colorRectangel.loadFromFile("../img/09-10/colors.png", sysInit.getRenderer());
    
    girl.loadFromFile("../img/09-10/Windows.png", sysInit.getRenderer());
    colorRectangel.setBlendMode(SDL_BLENDMODE_BLEND);

    SDL_Color textureColor = {0XFF, 0XFF, 0XFF, 0XFF};
    
    while (!eventsControl.getRunState())
    {
        eventsControl.events();

        SDL_SetRenderDrawColor(sysInit.getRenderer(), 0XFF, 0XFF, 0XFF, 0XFF);
        SDL_RenderClear(sysInit.getRenderer());

        modulation(eventsControl.getEventsRecord(), textureColor);

        girl.setColor(textureColor);
        girl.setAlpha(textureColor.a);

        colorRectangel.render(0, 0, sysInit.getRenderer());
        girl.render(0, 0, sysInit.getRenderer());

        SDL_RenderPresent(sysInit.getRenderer());

        frameControl(startFrameTick, frameRenderTime);
    }

    return EXIT_SUCCESS;
}

void modulation(const std::vector<bool> & __keyState, SDL_Color & __color)
{
    if (__keyState[SDL_SCANCODE_1]) { 
        __color.r += ADJUST_VALUE;
    }
    else if (__keyState[SDL_SCANCODE_2]) {
        __color.g += ADJUST_VALUE;
    }
    else if (__keyState[SDL_SCANCODE_3]) { 
        __color.b += ADJUST_VALUE;
    }
    else if (__keyState[SDL_SCANCODE_4]) { 
        __color.r -= ADJUST_VALUE;
    }
    else if (__keyState[SDL_SCANCODE_5]) { 
        __color.g -= ADJUST_VALUE;
    }
    else if (__keyState[SDL_SCANCODE_6]) { 
        __color.b -= ADJUST_VALUE;
    }
    else if (__keyState[SDL_SCANCODE_UP]) { 
        __color.a += ADJUST_VALUE;
    }
    else if (__keyState[SDL_SCANCODE_DOWN]){
        __color.a -= ADJUST_VALUE;
    }
    else if (__keyState[SDL_SCANCODE_0]) { 
        std::memset(&__color, 0xFF, sizeof(SDL_Color));
    }
}

void frameControl(Uint64 & __startFrameTick, const Uint64 __frameRenderTime)
{
    Uint64 currentTick = SDL_GetTicks64();
    Uint64 renderTime  = currentTick - __startFrameTick;

    if (renderTime < __frameRenderTime) { 
        SDL_Delay(__frameRenderTime - renderTime); 
    }

    __startFrameTick = SDL_GetTicks64();
}