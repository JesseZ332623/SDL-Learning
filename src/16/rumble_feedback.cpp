#include "../../SDLTemplateCode/include/SDLTemplate.hpp"

#define FPS 165     // 每秒钟帧数

int WinMain(int argc, char const * argv[])
{
    SystemInit::WindowSize windowSize = {800, 600};
    SystemInit sysInit(windowSize, "16. Game Controller Rumble Feedback");

    sysInit.init();

    SoundEffects        soundEffect(DEAULT_AUDIO_ATTRIBUTION);

    soundEffect.init(true);

    Uint64 startTick  = SDL_GetTicks64();
    Uint64 renderTick = STANDING_RENDER_TIME(startTick, FPS);

    EventsControl events;

    FontsTexture        rumbleInfoShown;

    RectangleTexture    rumbleValRect_L;
    FontsTexture        rumbleInfoShown_L;

    RectangleTexture    rumbleValRect_R;
    FontsTexture        rumbleInfoShown_R;

    SDL_Color           colorRumbleValRect = {0, 0xAB, 0xFF, 0xFF};

    rumbleInfoShown.open("../fonts/Consolas-Regular.ttf", 20);
    rumbleInfoShown_L.open("../fonts/Consolas-Regular.ttf", 20);
    rumbleInfoShown_R.open("../fonts/Consolas-Regular.ttf", 20);

    rumbleValRect_L.load(
                "Rumble Value L", 
                80, 255 * 1.5, 
                colorRumbleValRect, sysInit.getRenderer()
            );
    
    rumbleValRect_R.load(
                "Rumble Value R", 
                80, 255 * 1.5, 
                colorRumbleValRect, sysInit.getRenderer()
            );

    soundEffect.load("../audio/effects/Harp Gliss Up.wav");

    Uint64 soundPlayClocker = SDL_GetTicks64();

    while (!events.getRunstate())
    {
        events.recordEvents();

        SDL_RenderClear(sysInit.getRenderer());
        SDL_SetRenderDrawColor(sysInit.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);

        if (events.getTriggerValue().size() != 0)
        {
            rumbleInfoShown.load(
                events.getTriggerValue()[0].show(), 
                {0, 0, 0, 0xFF}, sysInit.getRenderer()
            );

            rumbleInfoShown_L.load(
                std::to_string(events.getTriggerValue()[0].leftTriggerVal) + " / 255",
                {0, 0, 0, 0xFF}, sysInit.getRenderer()
            );

            rumbleInfoShown_R.load(
                std::to_string(events.getTriggerValue()[0].rightTriggerVal) + " / 255",
                {0, 0, 0, 0xFF}, sysInit.getRenderer()
            );

            rumbleValRect_L.setAlpha(events.getTriggerValue()[0].leftTriggerVal);
            rumbleValRect_R.setAlpha(events.getTriggerValue()[0].rightTriggerVal);

            if (
                (events.getTriggerValue()[0].rightTriggerVal == 255) || 
                (events.getTriggerValue()[0].leftTriggerVal  == 255)
            )
            {
                Uint64 currentTime = SDL_GetTicks64();
                if ((currentTime - soundPlayClocker) > 500) {
                    soundEffect.play(0, 0);
                }

                soundPlayClocker = currentTime;
            }
        }
        else
        {
            rumbleInfoShown.load(
                "No Game Controller Connected.....",
                {0, 0, 0, 0xFF}, sysInit.getRenderer()
            );
        }

        rumbleInfoShown.render(0, 0, sysInit.getRenderer());

        rumbleValRect_L.render(sysInit.getRenderer(), 150, 100, RectangleTexture::WHOLE);
        rumbleInfoShown_L.render(153, 100 + rumbleValRect_L.getRenderPosition().h + 5, sysInit.getRenderer());

        rumbleValRect_R.render(sysInit.getRenderer(), 550, 100, RectangleTexture::WHOLE);
        rumbleInfoShown_R.render(553, 100 + rumbleValRect_R.getRenderPosition().h + 5, sysInit.getRenderer());

        SDL_RenderPresent(sysInit.getRenderer());

        frameControl(startTick, renderTick);
    }

    return EXIT_SUCCESS;
}