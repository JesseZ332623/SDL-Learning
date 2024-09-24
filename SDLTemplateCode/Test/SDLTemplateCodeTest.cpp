#include "../../SDLTemplateCode/include/SDLTemplate.hpp"

#define RECTENGLE_SIDE_LENGTH 250
#define CIRCLE_RADIUS         RECTENGLE_SIDE_LENGTH / 2

#define FRAME_WIDTH  64
#define FRAME_HEIGHT 205

#define FPS 3

int WinMain(int argc, char const * argv[])
{
    system("cls");

    SystemInit::WindowSize windowSize = {650, 650};
    SystemInit      sysInit(windowSize, "DL TemplateCode Test");

    EventsControl   eventsControl;

    TextureImage        image;
    CircleTexture       circleA;
    RectangleTexture    rectangleA;

    sysInit.init();

    Uint64 startTick  = SDL_GetTicks64();
    Uint64 renderTick = startTick / FPS;

    // 加载火柴人纹理
    image.load("../img/11/stick_figuer.png", {0x00, 0xFF, 0XFF}, SDL_TRUE, sysInit.getRenderer());

    // 由于纹理是要旋转的，所以需要计算纹理旋转所覆盖的大小（矩形的长和宽）
    int rectWidth  = 1 + 2 * static_cast<int>(std::sqrt(std::pow(FRAME_WIDTH, 2) + std::pow(FRAME_HEIGHT, 2)));
    int rectHeight = rectWidth;

    // 定位矩形到屏幕中间。
    int rectX = windowSize.w / 2 - rectWidth  / 2;
    int rectY = windowSize.h / 2 - rectHeight / 2;

    SDL_Rect framePos = {0, 0, FRAME_WIDTH, FRAME_HEIGHT};

    /**
     * @brief 纹理旋转属性：起始角度 360，旋转中心点（0, 0），水平翻转
    */
    TextureImage::FilpAttribution flip = {360.0, {0, 0}, SDL_FLIP_HORIZONTAL};

    rectangleA.load(
        "Rectengle A", 
        rectWidth, rectHeight, {0, 0, 0 ,0XFF}, sysInit.getRenderer()
    );
    circleA.load("Circle A", {rectX + rectWidth / 2, rectY + rectHeight / 2, rectWidth / 2});

    while (!eventsControl.getRunstate())
    {
        eventsControl.recordEvents();

        if (flip.angle <= 0.0) { flip.angle = 360.0; }
        if (framePos.x >= image.getRenderPosition().w) { framePos.x = 0; }

        SDL_SetRenderDrawColor(sysInit.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(sysInit.getRenderer());

        rectangleA.render(
            sysInit.getRenderer(), rectX, rectY, RectangleTexture::BORDER
        );

        circleA.render(
            0xFFFF, {0x00, 0x00, 0x00}, 
            sysInit.getRenderer(), CircleTexture::BORDER
        );

        image.render(
            windowSize.w / 2, windowSize.h / 2, 
            sysInit.getRenderer(), framePos, flip
        );

        SDL_RenderPresent(sysInit.getRenderer());

        flip.angle -= 5;
        framePos.x += FRAME_WIDTH;

        frameControl(startTick, renderTick);
    }

    return EXIT_SUCCESS;
}