# if true
#include "../../SDLTemplateCode/include/SDLTemplate.hpp"

#define RECTENGLE_SIDE_LENGTH 250
#define CIRCLE_RADIUS         RECTENGLE_SIDE_LENGTH / 2

int WinMain(int argc, char const * argv[])
{
    system("cls");

    SystemInit::WindowSize windowSize = {650, 650};
    SystemInit      sysInit;

    EventsControl   eventsControl;

    CircleTexture       circleA;
    RectengleTexture    rectengleA;

    sysInit.init(windowSize, "12. SDL TemplateCode Test");

    int rectX = windowSize.w / 2 - RECTENGLE_SIDE_LENGTH / 2;
    int rectY = windowSize.h / 2 - RECTENGLE_SIDE_LENGTH / 2;

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
        eventsControl.eventsRecord();

        SDL_SetRenderDrawColor(sysInit.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(sysInit.getRenderer());

        rectengleA.render(
            rectX, rectY, 
            {0x65, 0xCB, 0xF6}, sysInit.getRenderer()
        );
        circleA.render(0xFFFF, {0x00, 0XAD, 0x12}, sysInit.getRenderer());
        circleA.fill({0x00, 0XAD, 0x12}, sysInit.getRenderer());

        SDL_RenderPresent(sysInit.getRenderer());
    }

    return EXIT_SUCCESS;
}
#endif