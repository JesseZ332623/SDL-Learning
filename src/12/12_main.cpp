#include "../../SDLTemplateCode/include/SDLTemplate.hpp"

#define FPS 165

#define ANGLE_ADJUST 2.000

/**
 * @brief 纹理的旋转控制，通过按下不同的键来控制键头纹理的旋转等属性，
 *        键位部署如下：
 * 
 * @brief - LEFT  (<-)  键头向左旋转
 * @brief - RIGHT (->)  键头向右旋转
 * @brief - X           切换翻转模式为垂直翻转
 * @brief - Y           切换翻转模式为水平翻转
 * @brief - SPACE       复原键头到初始状态
 * 
 * @param __flip            纹理的旋转属性
 * @param __keyboardState   键位状态记录
*/
void rotateControl(
    TextureImage::FilpAttribution & __flip, 
    const EventsControl::KeyCodeMap & __keyboardState
);

int WinMain(int argc, char const * argv[])
{
    system("cls");

    SystemInit::WindowSize windowSize = {600, 600}; 
    SystemInit sysInit(windowSize, "12. Arrow flip and rotate");

    TextureImage  arrow;
    CircleTexture circle;

    EventsControl events;

    sysInit.init();

    Uint64 startTick  = SDL_GetTicks64();
    Uint64 renderTick = startTick / FPS;

    // 加载键头纹理，透明化纹理的青色背景。
    arrow.load("../img/12/arrow.png", {0, 255, 255}, SDL_TRUE, sysInit.getRenderer());
    arrow.setColor({0x1F, 0xDA, 0xBB});
    arrow.setAlpha(0xA0);
    SDL_Rect arrowRect = arrow.getRenderPosition();

    circle.load("Surround arrow", {windowSize.w / 2, windowSize.h / 2, 145});

    // 固定键头的渲染位置在窗口正中
    int arrowX = windowSize.w / 2 - arrowRect.w / 2;
    int arrowY = windowSize.h / 2 - arrowRect.h / 2;

    // 键头的选择信息
    TextureImage::FilpAttribution flip = {
        0, {arrowRect.w / 2, arrowRect.h / 2}, 
        SDL_FLIP_NONE
    };
    
    while (!events.getRunstate())
    {
        events.recordEvents();
        rotateControl(flip, events.getKeyboardState());

        SDL_SetRenderDrawColor(sysInit.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(sysInit.getRenderer());

        arrow.render(arrowX, arrowY, sysInit.getRenderer(), arrow.defaultClip(), flip);
        circle.render(0xFFFF, {0, 0, 0}, sysInit.getRenderer(), CircleTexture::BORDER);

        SDL_RenderPresent(sysInit.getRenderer());

        frameControl(startTick, renderTick);
    }

    return EXIT_SUCCESS;
}

void rotateControl(TextureImage::FilpAttribution & __flip, const EventsControl::KeyCodeMap & __keyboardState)
{
    auto CONDITIONS = [&](SDL_Scancode KeyCode) {
        return (__keyboardState.find(KeyCode) != __keyboardState.end()) &&
               (__keyboardState.find(KeyCode)->second);
    };

    if (CONDITIONS(SDL_SCANCODE_LEFT)) 
    {
        __flip.angle -= ANGLE_ADJUST;
        if (__flip.angle <= -360) { __flip.angle = 0; }
    }
    else if (CONDITIONS(SDL_SCANCODE_RIGHT)) 
    {
        __flip.angle += ANGLE_ADJUST;
        if (__flip.angle >= 360) { __flip.angle = 0; }
    }
    else if (CONDITIONS(SDL_SCANCODE_X)) 
    {
        __flip.flipFlag = SDL_FLIP_HORIZONTAL; 
    }
    else if (CONDITIONS(SDL_SCANCODE_Y))
    {
        __flip.flipFlag = SDL_FLIP_VERTICAL; 
    }
    else if (CONDITIONS(SDL_SCANCODE_SPACE))
    {
        __flip.angle = 0;
        __flip.flipFlag = SDL_FLIP_NONE;
    }
}