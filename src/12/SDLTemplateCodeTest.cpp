# if true
#include "../../SDLTemplateCode/include/SDLTemplate.hpp"

#define RECTENGLE_SIDE_LENGTH 250
#define CIRCLE_RADIUS         RECTENGLE_SIDE_LENGTH / 2

#define FRAME_WIDTH  64
#define FAMRE_HEIGHT 205

int WinMain(int argc, char const * argv[])
{
    system("cls");

    SystemInit::WindowSize windowSize = {650, 650};
    SystemInit      sysInit(windowSize, "12. SDL TemplateCode Test");

    EventsControl   eventsControl;

    TextureImage        image;
    CircleTexture       circleA;
    RectengleTexture    rectengleA;

    sysInit.init();

    int rectX = windowSize.w / 2 - RECTENGLE_SIDE_LENGTH / 2;
    int rectY = windowSize.h / 2 - RECTENGLE_SIDE_LENGTH / 2;

    SDL_Rect framePos = {0, 0, FRAME_WIDTH, FAMRE_HEIGHT};
    image.load("../img/11/stick_figuer.png", sysInit.getRenderer());
    rectengleA.load(
        "Rectengle A", 
        RECTENGLE_SIDE_LENGTH, RECTENGLE_SIDE_LENGTH, 
        sysInit.getRenderer()
    );
    circleA.load(
        "Circle A", 
        {
            rectX + RECTENGLE_SIDE_LENGTH / 2, 
            rectY + RECTENGLE_SIDE_LENGTH / 2, 
            CIRCLE_RADIUS
        }
    );

    while (!eventsControl.getRunstate())
    {
        eventsControl.recordEvents();

        SDL_SetRenderDrawColor(sysInit.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(sysInit.getRenderer());

        image.render(0, 0, framePos, sysInit.getRenderer());
        rectengleA.render(
            rectX, rectY, {0x65, 0xCB, 0xF6}, 
            sysInit.getRenderer(), RectengleTexture::WHOLE
        );
        circleA.render(0xFFFF, {0x00, 0XAD, 0x12}, sysInit.getRenderer());
        circleA.fill({0x00, 0XAD, 0x12}, sysInit.getRenderer());

        SDL_RenderPresent(sysInit.getRenderer());
    }

    return EXIT_SUCCESS;
}
#endif