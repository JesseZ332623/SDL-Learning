#include "../../include/08.hpp"

/**
 * @brief 根据键盘事件哈希表 __keyState 
 *        的记录来改变图像的渲染位置，纹理的裁剪位置等。
 * 
 * @param __keyState    键盘事件哈希表
 * @param __texturePos  纹理相对于屏幕的渲染位置信息
 * @param __clipPos     相对于整个纹理的裁剪位置信息
 * @param __speed       移速
*/
void moveOperator(
    const std::vector<bool> & __keyState, 
    SDL_Rect & __texturePos, SDL_Rect & __clipPos, const int __speed
);

/**
 * @brief 帧率的控制
 * 
 * @param __frameStart  某一帧开始渲染的时间刻
 * @param __frameTime   维持指定帧数渲染一帧所用的时间
*/
void frameControl(Uint64 & __frameStart, const Uint64 __frameTime);

int WinMain(int argc, char const *argv[])
{
    using namespace MyLib::MyLoger;

    system("cls");

    SystemInit    sysInit;
    Texture       circles;
    EventsControl eventsControl;

    sysInit.init();
    circles.loadFromFile("../img/08/circles.png", sysInit.getRenderer());

    SDL_Rect originalPos = {0};
    SDL_Rect clipPos = {0, 0, CIRCLE_DIAMETER, CIRCLE_DIAMETER};

    Uint64 frameStartTick = SDL_GetTicks64();
    Uint64 farameTime     = frameStartTick / FPS;

#if true
    while (!eventsControl.getRunState())
    {
        eventsControl.events();
        moveOperator(eventsControl.getEventsRecord(), originalPos, clipPos, SPEED);

        SDL_SetRenderDrawColor(sysInit.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(sysInit.getRenderer());

        //printf("(x, y) = (%d, %d)\n", originalPos.x, originalPos.y);
        circles.render(
            originalPos.x, originalPos.y, 
            clipPos, 
            sysInit.getRenderer()
        );

        SDL_RenderPresent(sysInit.getRenderer());

        frameControl(frameStartTick, farameTime);
    }
#endif

    return EXIT_SUCCESS;
}

void moveOperator(
    const std::vector<bool> & __keyState, 
    SDL_Rect & __texturePos, SDL_Rect & __clipPos, const int __speed
) {
    if (__keyState.at(SDL_SCANCODE_W)) { 
        __clipPos.x = __clipPos.y = 0;
        __texturePos.y -= __speed; 
    }
    else if (__keyState.at(SDL_SCANCODE_A)) {
        __clipPos.x = CIRCLE_DIAMETER;
        __clipPos.y = 0;
        __texturePos.x -= __speed; 
    }
    else if (__keyState.at(SDL_SCANCODE_S)) { 
        __clipPos.x = 0;
        __clipPos.y = CIRCLE_DIAMETER;
        __texturePos.y += __speed; 
    }
    else if (__keyState.at(SDL_SCANCODE_D)) { 
        __clipPos.x = __clipPos.y = CIRCLE_DIAMETER;
        __texturePos.x += __speed; 
    }
}

void frameControl(Uint64 & __frameStart, const Uint64 __frameTime)
{
    Uint64 currentTick = SDL_GetTicks64();              // 当前时间刻

    /**
     * @brief 从这一帧渲染开始的时间，到渲染完成后所用的时间
    */
    Uint64 passedTick  = currentTick - __frameStart;

    /**
     * @brief 如果这一帧渲染过快（小于标准的一帧渲染时间）
    */
    if (passedTick < __frameTime) { 
        // 就需要暂停程序，补上这一点时间
        SDL_Delay(__frameTime - passedTick); 
    }

    // 更新某一帧开始渲染的时间刻
    __frameStart = SDL_GetTicks64();
}