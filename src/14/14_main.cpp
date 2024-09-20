#include "../../SDLTemplateCode/include/SDLTemplate.hpp"

#define FPS 165

#define RECTANGLE_WIDTH  198
#define RECTANGLE_HEIGHT 187

/**
 * @brief 根据不同的鼠标事件，改变矩形的裁剪或渲染位置。
 * 
 * @param __mouseEvents     传入整个事件类，但是函数内只调用和鼠标事件相关的方法。
 * @param __pos             矩形位置
 * @param __clip            矩形裁剪范围
 */
void mouseEventsControl(const EventsControl & __mouseEvents, SDL_Point & __pos, SDL_Rect & __clip);

int WinMain(int argc, char const * argv[])
{
    system("cls");

    SystemInit::WindowSize windowSize = {1050, 850};
    SystemInit    sysInit(windowSize, "14. Mouse Events");

    sysInit.init();
    Uint64 startTick  = SDL_GetTicks64();
    Uint64 renderTime = STANDING_RENDER_TIME(startTick, FPS);

    EventsControl events;

    FontsTexture  fpsShown;
    TextureImage  rectangles;

    SDL_Rect  rectClip = {0, 0, RECTANGLE_WIDTH, RECTANGLE_HEIGHT};
    SDL_Point rectPosition = {50, 86};

    fpsShown.open("../fonts/Consolas-Regular.ttf", 15);
    rectangles.load("../img/14/rectengles.png", {}, SDL_FALSE, sysInit.getRenderer());

    while (!events.getRunstate())
    {
        events.recordEvents();
        mouseEventsControl(events, rectPosition, rectClip);
        SDL_SetRenderDrawColor(sysInit.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(sysInit.getRenderer());

        rectangles.render(rectPosition.x, rectPosition.y, sysInit.getRenderer(), rectClip);

        SDL_RenderPresent(sysInit.getRenderer());

        frameControl(startTick, renderTime);
    }

    return EXIT_SUCCESS;
}

void mouseEventsControl(
    const EventsControl & __mouseEvents, SDL_Point & __pos, SDL_Rect & __clip
) {
    const EventsControl::MouseButtonMap & mouseButtonMap = __mouseEvents.getMouseState();
    const SDL_Point & mousePos = __mouseEvents.getMousePosition();

    SDL_Point rectRightBorder = {__pos.x + RECTANGLE_WIDTH, __pos.y + RECTANGLE_HEIGHT};

/**
  * @brief 查询特定鼠标按键事件的标志位。
*/
#define CONDITIONS(MouseKeyCode) (                                  \
    (mouseButtonMap.find(MouseKeyCode) != mouseButtonMap.end()) &&  \
    (mouseButtonMap.find(MouseKeyCode)->second)                     \
)

/**
  * @brief 鼠标位置限制，确保鼠标位置在纹理大小范围内时才进行相应操作。
*/
#define MOUSEPOS_ASTRICT ((mousePos <= rectRightBorder) && (mousePos >= __pos))

    // 鼠标左键按下并且鼠标位置在纹理范围内时，更新裁剪范围。
    if (CONDITIONS(SDL_BUTTON_LEFT) && MOUSEPOS_ASTRICT) {
        __clip.x = RECTANGLE_WIDTH;
    }
    // 鼠标右键按下并且鼠标位置在纹理范围内时
    else if (CONDITIONS(SDL_BUTTON_RIGHT) && MOUSEPOS_ASTRICT) {
        // 且在鼠标移动的情况下
        if (!__mouseEvents.getMouseMotionState()) {
            __clip.x = RECTANGLE_WIDTH * 2;
            __pos.x = mousePos.x - RECTANGLE_WIDTH / 2;
            __pos.y = mousePos.y - RECTANGLE_HEIGHT / 2;
        }
    }
    else { __clip.x = 0; }
}