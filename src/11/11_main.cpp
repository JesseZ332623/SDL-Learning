#include "../../include/11/systemInit.hpp"
#include "../../include/11/texture.hpp"
#include "../../include/11/events.hpp"
#include "../../include/11/defs.hpp"

/**
 * @brief 维持帧数在一个固定值。
 * 
 * @param __startFrameTick       某一帧开始渲染前的时间刻
 * @param __frameRenderTime      维持一定帧数渲染一帧所需要的标准时间
*/
void frameControl(Uint64 & __startFrameTick, const Uint64 __frameRenderTime);

int WinMain(int argc, char const * argv[])
{
    system("cls");
    
    SystemInit      sysInit;
    EventsControl   events;
    TextureImage    stickFiguer;

    sysInit.init();

    Uint64 startFrameTime  = SDL_GetTicks64();
    Uint64 renderFrameTime = startFrameTime / FPS;

    stickFiguer.load("../img/11/stick_figuer.png", sysInit.getRenderer());

#if false
    printf(
        "stick figuer texture width = %d, height = %d\n", 
        stickFiguer.getRenderPosition().w, stickFiguer.getRenderPosition().h
    );
#endif

    SDL_Rect framePos = {0, 0, FRAME_WIDTH, FAMRE_HEIGHT};

    while (!events.getRunstate())
    {
        events.eventsRecord();

        if (framePos.x >= stickFiguer.getRenderPosition().w) {
            framePos.x = 0;
        }

        SDL_SetRenderDrawColor(sysInit.getRenderer(), 0x00, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(sysInit.getRenderer());

        stickFiguer.render(
            SCREEN_WIDTH / 2 - FRAME_WIDTH, SCREEN_HEIGHT / 2 - FAMRE_HEIGHT / 2, 
            framePos, sysInit.getRenderer()
        );

        SDL_RenderPresent(sysInit.getRenderer());
        framePos.x += FRAME_WIDTH;

        frameControl(startFrameTime, renderFrameTime);
    }

    return EXIT_SUCCESS;
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