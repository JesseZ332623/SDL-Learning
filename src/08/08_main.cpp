#include "../../include/08.hpp"

int WinMain(int argc, char const *argv[])
{
    using namespace MyLib::MyLoger;

    system("cls");

    SystemInit    sysInit;
    Texture       circles;
    EventsControl eventsControl;

    sysInit.init();
    circles.loadFromFile("../img/08/circles.png", sysInit.getRenderer());

    while (!eventsControl.getInfoRecord().quit)
    {
        eventsControl.events();

        SDL_SetRenderDrawColor(sysInit.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(sysInit.getRenderer());

        circles.render(
            0, 0, {0, 0, CIRCLE_DIAMETER, CIRCLE_DIAMETER}, sysInit.getRenderer()
        );

        circles.render(
            SCREEN_WIDTH - CIRCLE_DIAMETER * 2, 0, 
            {CIRCLE_DIAMETER, 0, CIRCLE_DIAMETER, CIRCLE_DIAMETER}, sysInit.getRenderer()
        );

        circles.render(
            0, SCREEN_HEIGHT - CIRCLE_DIAMETER * 2,
            {0, CIRCLE_DIAMETER, CIRCLE_DIAMETER, CIRCLE_DIAMETER}, 
            sysInit.getRenderer()
        );

        circles.render(
            SCREEN_WIDTH - CIRCLE_DIAMETER * 2, SCREEN_HEIGHT - CIRCLE_DIAMETER * 2,
            {CIRCLE_DIAMETER, CIRCLE_DIAMETER, CIRCLE_DIAMETER, CIRCLE_DIAMETER},
            sysInit.getRenderer()
        );

        SDL_RenderPresent(sysInit.getRenderer());
    }

    return EXIT_SUCCESS;
}