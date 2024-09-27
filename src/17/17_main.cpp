/**
 * @file 17_main.cpp
 * 
 * @brief SDL_mixer 模块音频与音效的播放。
 * 
 * @date 2024-09-27
 * 
 * @copyright Copyright (c) 2024
 * 
*/

#include "../../SDLTemplateCode/include/SDLTemplate.hpp"

#ifdef main
#undef main
#undef __argc
#endif

#define FPS 8              // 游戏帧数

#define ANIMITION_COUNTS            4   // 要加载的动画数量

#define ANIMITION_FRAMES            8   // 每组动画的帧数
#define FRAME_WIDTH                 29  // 帧宽
#define FRAME_WIDTH_COMPENSATE      7   // 帧宽补偿
#define FRAME_HEIGHT                48  // 帧高

#define SPEED  4

class BobbyWalkSoundEffects
{
    public:
        const char * imagePath[ANIMITION_COUNTS] = 
        {
            "../img/17/bobby_up.png",
            "../img/17/bobby_left.png",
            "../img/17/bobby_right.png",
            "../img/17/bobby_down.png"
        };

        enum RenderingIndex {
            UP = 0, LEFT, RIGHT, DOWN
        };
    private:
        SystemInit::WindowSize  windowSize;
        SystemInit              sysInit;

        Audio                   backgroundMusic;
        SoundEffects            walkSound;

        EventsControl           events;

        std::vector<TextureImage> bobbyMotion;

        RenderingIndex          renderingIndex;
        SDL_Point               renderPos;
        SDL_Rect                clipPos;
        int                     framePlay;

        Uint64                  startTick;
        Uint64                  renderTick;

        void keyboardEventsHandle(void);

    public:
        BobbyWalkSoundEffects(void);

        void init(void);

        void load(void);

        void run(void);

        ~BobbyWalkSoundEffects() = default;
};

void BobbyWalkSoundEffects::keyboardEventsHandle(void)
{
    if (this->events.getKeyboardState().size() == 0) { return; }

    for (auto & [keyCode, keyState] : this->events.getKeyboardState())
    {
        if (keyState.pressed)
        {
            switch (keyCode)
            {
                case SDL_SCANCODE_W:
                    this->renderingIndex = UP;
                    this->renderPos.y -= SPEED;
                    
                    if (framePlay == ANIMITION_FRAMES) { framePlay = 0; }

                    this->clipPos.x += (FRAME_WIDTH + FRAME_WIDTH_COMPENSATE);

                    if (framePlay == ANIMITION_COUNTS) { this->clipPos.x += FRAME_WIDTH; }
                    if (clipPos.x >= this->bobbyMotion.at(UP).getRenderPosition().w) {
                                                    clipPos.x = 0;
                                                }
                    break;
                
                case SDL_SCANCODE_A:
                    this->renderingIndex = LEFT;
                    this->renderPos.x -= SPEED;

                    if (framePlay == ANIMITION_FRAMES) { framePlay = 0; }

                    this->clipPos.x += (FRAME_WIDTH + FRAME_WIDTH_COMPENSATE);

                    if (framePlay == ANIMITION_COUNTS) { this->clipPos.x += FRAME_WIDTH; }
                    if (clipPos.x >= this->bobbyMotion.at(UP).getRenderPosition().w) {
                                                    clipPos.x = 0;
                                                }                    
                    break;
                
                case SDL_SCANCODE_S:
                    this->renderingIndex = DOWN;
                    this->renderPos.y += SPEED;

                    if (framePlay == ANIMITION_FRAMES) { framePlay = 0; }

                    this->clipPos.x += (FRAME_WIDTH + FRAME_WIDTH_COMPENSATE);

                    if (framePlay == ANIMITION_COUNTS) { this->clipPos.x += FRAME_WIDTH; }
                    if (clipPos.x >= this->bobbyMotion.at(UP).getRenderPosition().w) {
                                                    clipPos.x = 0;
                                                }                    
                    
                    break;

                case SDL_SCANCODE_D:
                    this->renderingIndex = RIGHT;
                    this->renderPos.x += SPEED;

                    if (framePlay == ANIMITION_FRAMES) { framePlay = 0; }

                    this->clipPos.x += (FRAME_WIDTH + FRAME_WIDTH_COMPENSATE);

                    if (framePlay == ANIMITION_COUNTS) { this->clipPos.x += FRAME_WIDTH; }
                    if (clipPos.x >= this->bobbyMotion.at(UP).getRenderPosition().w) {
                                                    clipPos.x = 0;
                                                }
                    
                    break;

                default:
                    break;
            }
        }
    }
}

BobbyWalkSoundEffects::BobbyWalkSoundEffects(void) : 
windowSize({800, 800}), sysInit(windowSize, "17. Bobby Walk"), 
backgroundMusic(DEAULT_AUDIO_ATTRIBUTION), walkSound(DEAULT_AUDIO_ATTRIBUTION), 
bobbyMotion(ANIMITION_COUNTS), renderingIndex(DOWN), framePlay(1) {}


void BobbyWalkSoundEffects::init(void)
{
    sysInit.init();
    backgroundMusic.init();
    walkSound.init(false);

    startTick  = SDL_GetTicks64();
    renderTick = STANDING_RENDER_TIME(startTick, FPS);
}

void BobbyWalkSoundEffects::load(void)
{
    backgroundMusic.load("../audio/on the ground.mp3");
    walkSound.load("../audio/effects/walk.wav");
    
    for (int index = 0; index < ANIMITION_COUNTS; ++index)
    {
        bobbyMotion.at(index).load(
                                imagePath[index], {}, SDL_FALSE, 
                                sysInit.getRenderer()
                            );
    }
}

void BobbyWalkSoundEffects::run()
{
    this->backgroundMusic.setVolume(100);
    this->backgroundMusic.play(-1);

    this->renderPos = {100, 100};
    this->clipPos = {0, 0, FRAME_WIDTH, FRAME_HEIGHT};

    while (!this->events.getRunstate())
    {
        this->events.recordEvents();
        this->keyboardEventsHandle();

        SDL_RenderClear(this->sysInit.getRenderer());
        SDL_SetRenderDrawColor(this->sysInit.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);

        switch (this->renderingIndex)
        {
            case UP:
                this->bobbyMotion.at(UP).render(
                                renderPos.x, renderPos.y, 
                                sysInit.getRenderer(), clipPos
                            );
                break;
            
            case LEFT:
                this->bobbyMotion.at(LEFT).render(
                                renderPos.x, renderPos.y, 
                                sysInit.getRenderer(), clipPos
                            );
                break;

            case RIGHT:
                this->bobbyMotion.at(RIGHT).render(
                                renderPos.x, renderPos.y, 
                                sysInit.getRenderer(), clipPos
                            );
                break;

            case DOWN:
                this->bobbyMotion.at(DOWN).render(
                                renderPos.x, renderPos.y, 
                                sysInit.getRenderer(), clipPos
                            );
                break;
            
            default:
                break;
        }

        SDL_RenderPresent(this->sysInit.getRenderer());

        ++framePlay;

        frameControl(this->startTick, this->renderTick);
    }
}

int main(int argc, char const * argv[])
{
    system("cls");

    BobbyWalkSoundEffects bobbyWalkSoundEffects;

    bobbyWalkSoundEffects.init();
    bobbyWalkSoundEffects.load();
    bobbyWalkSoundEffects.run();
    
    return EXIT_SUCCESS;
}