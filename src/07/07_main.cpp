#include "../../include/07.hpp"

int WinMain(int argc, char const * argv[])
{
    using namespace MyLib::MyLoger;

    system("cls");
    
    SystemInit      sysInit;
    EventsControl   eventsControl;
    Texture         background;
    Texture         character;

    sysInit.init();

    if (!background.loadFromFile("../img/07/background.png", sysInit.getRenderer())) {
        ERROR_LOG("Faild to load background.\n");
    }
    else if (!character.loadFromFile("../img/07/foo.png", sysInit.getRenderer())) {
        ERROR_LOG("Faild to load charecter.\n");
    }
    else {
        while (!eventsControl.getInfoRecord().quit)
        {
            eventsControl.events();

            SDL_SetRenderDrawColor(sysInit.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(sysInit.getRenderer());

            background.render(0, 0, sysInit.getRenderer());
            character.render(240, 190, sysInit.getRenderer());

            SDL_RenderPresent(sysInit.getRenderer());
        }
    }

    return EXIT_SUCCESS;
}