#include "../../SDLTemplateCode/include/SDLTemplate.hpp"

#include <vector>

#include <cstdlib>
#include <ctime>

#define RECTENGLE_SIDE_LENGTH 250

#define FPS 120

int WinMain(int argc, char const * argv[])
{
    system("cls");

    srand((unsigned int)std::time(nullptr));

    SystemInit::WindowSize windowSize = {450, 450};
    SystemInit      sysInit(windowSize, "SDL TemplateCode Test 2");

    EventsControl   eventsControl;

    std::vector<RectangleTexture> rectangles(15);
    std::vector<RectangleTexture::FilpAttribution> flips;

    sysInit.init();

    Uint64 startTick  = SDL_GetTicks64();
    Uint64 renderTick = startTick / FPS;

    SDL_Color rectColor = {0, 0, 0, 0xFF};
    RectangleTexture::FilpAttribution flip = {
        0, {0, 0}, SDL_FLIP_HORIZONTAL
    };
    int number = 1;
    std::string rectName = "Rotate Rectangle_";

    for (RectangleTexture & rect : rectangles)
    {
        rectColor.r += rand() % 254 + 1;
        rectColor.g += rand() % 254 + 1;
        rectColor.b += rand() % 254 + 1;
        rectName += std::to_string(number);

        rect.load(
            rectName, 
            300, 300, rectColor, sysInit.getRenderer()
        );

        rectName.erase(rectName.find('_') + 1);

        flip.center.x = rect.getRenderPosition().w / 2;
        flip.center.y = rect.getRenderPosition().h / 2;
        flips.push_back(flip);
        flip.angle += 15;
        ++number;
    }

    int renderX = windowSize.w / 2 - rectangles[0].getRenderPosition().w / 2;
    int renderY = windowSize.h / 2 - rectangles[0].getRenderPosition().h / 2;

    while (!eventsControl.getRunstate())
    {
        eventsControl.recordEvents();

        SDL_SetRenderDrawColor(sysInit.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(sysInit.getRenderer());

        for (std::size_t index = 0; index < rectangles.size(); ++index)
        {
            rectangles[index].render(
                sysInit.getRenderer(), 
                renderX, renderY, RectangleTexture::FILLED, flips[index]
            );

            flips[index].angle += 2.00;
        }

        SDL_RenderPresent(sysInit.getRenderer());

        frameControl(startTick, renderTick);
    }

    return EXIT_SUCCESS;
}