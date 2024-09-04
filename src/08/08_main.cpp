#include "../../include/08.hpp"

/**
 * @brief 
 * 
 * @param __keyState 
 * @param __texturePos 
*/
void moveOperator(
    const std::vector<bool> & __keyState, 
    SDL_Rect & __texturePos, const int __speed
);

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

#if true
    while (!eventsControl.getRunState())
    {
        eventsControl.events();
        moveOperator(eventsControl.getEventsRecord(), originalPos, SPEED);

        SDL_SetRenderDrawColor(sysInit.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(sysInit.getRenderer());

        //printf("(x, y) = (%d, %d)\n", originalPos.x, originalPos.y);
        circles.render(
            originalPos.x, originalPos.y, 
            {0, 0, CIRCLE_DIAMETER, CIRCLE_DIAMETER}, 
            sysInit.getRenderer()
        );

        SDL_RenderPresent(sysInit.getRenderer());
    }
#endif

    return EXIT_SUCCESS;
}

void moveOperator(
    const std::vector<bool> & __keyState, 
    SDL_Rect & __texturePos, const int __speed
) {
    if (__keyState.at(SDL_SCANCODE_W)) { __texturePos.y -= __speed; }
    else if (__keyState.at(SDL_SCANCODE_A)) { __texturePos.x -= __speed; }
    else if (__keyState.at(SDL_SCANCODE_S)) { __texturePos.y += __speed; }
    else if (__keyState.at(SDL_SCANCODE_D)) { __texturePos.x += __speed; }
}