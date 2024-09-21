#include "../../SDLTemplateCode/include/SDLTemplate.hpp"

#define FPS 165

int WinMain(int argc, char const *argv[])
{
    SystemInit::WindowSize windowSize = {1500, 1000};
    SystemInit sysInit(windowSize, "15 SDL Game Controller");

    EventsControl events;

    sysInit.init();

    Uint64 startTick = SDL_GetTicks64();
    Uint64 renderTick = STANDING_RENDER_TIME(startTick, FPS);

    FontsTexture rokersPosShown;
    FontsTexture arrowAngleShown;
    TextureImage arrow;

    rokersPosShown.open("../fonts/Consolas-Regular.ttf", 20);
    arrowAngleShown.open("../fonts/Consolas-Regular.ttf", 20);
    arrow.load("../img/12/arrow.png", {0, 0xFF, 0XFF, 0XFF}, SDL_TRUE, sysInit.getRenderer());

    SDL_Point arrowPos = {50, 50};
    int speed = 5;
    TextureImage::FilpAttribution flip = {
        0, {arrow.getRenderPosition().w / 2, arrow.getRenderPosition().h / 2}, SDL_FLIP_HORIZONTAL};

    SDL_Point rokersPosShownRenderPos = {0, 0};

    while (!events.getRunstate())
    {
        events.recordEvents();

        SDL_RenderClear(sysInit.getRenderer());
        SDL_SetRenderDrawColor(sysInit.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);

        arrow.render(
            arrowPos.x, arrowPos.y,
            sysInit.getRenderer(), arrow.defaultClip(), flip
        );

        rokersPosShown.render(
                rokersPosShownRenderPos.x, rokersPosShownRenderPos.y,
                sysInit.getRenderer()
        );

        arrowAngleShown.render(0, 30, sysInit.getRenderer());

#if true
        if (events.getRockersPosition().size() != 0)
        {
            rokersPosShown.load(
                events.getRockersPosition()[0].show(),
                {0, 0, 0, 0xFF}, sysInit.getRenderer()
            );

            arrowAngleShown.load(
                "Arrow angle: " + std::to_string(flip.angle) + 
                " Arrow Position: (" + std::to_string(arrowPos.x) + ", " + std::to_string(arrowPos.y) + ")",
                {0, 0, 0, 0xFF}, sysInit.getRenderer()
            );

            arrowPos.x += events.getRockersPosition()[0].leftRokerPos.x * speed;
            arrowPos.y += events.getRockersPosition()[0].leftRokerPos.y * speed;

            flip.angle = std::atan2(
                            events.getRockersPosition()[0].leftRokerPos.y,
                            events.getRockersPosition()[0].leftRokerPos.x
                        ) * (180.0F / M_PI);


            // 调整角度到 0 到 360 度之间
            if (flip.angle < 0.0F) { flip.angle += 360.0F; }

            if (arrowPos.x < 0) { arrowPos.x = 0; }
            if (arrowPos.x > (windowSize.w - arrow.getRenderPosition().w)) { 
                arrowPos.x = windowSize.w - arrow.getRenderPosition().w; 
            }
            if (arrowPos.y < 0) { arrowPos.y = 0; }
            if (arrowPos.y > (windowSize.h - arrow.getRenderPosition().h)) {
                arrowPos.y = windowSize.h - arrow.getRenderPosition().h;
            }
        }
        else 
        {
            rokersPosShown.load(
                "No Game Controller Connected.....",
                {0, 0, 0, 0xFF}, sysInit.getRenderer()
            );
        }
#endif
        SDL_RenderPresent(sysInit.getRenderer());
        frameControl(startTick, renderTick);
    }

    return EXIT_SUCCESS;
}