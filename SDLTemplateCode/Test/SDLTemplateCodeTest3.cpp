/**
 * @file SDLTemplateCodeTest3.cpp
 * @date 2024-09-25
 * 
 * @brief 对 Audio BGM 播放类进行测试。
 * 
 * @copyright Copyright (c) 2024
 * 
*/

#include "../../SDLTemplateCode/include/SDLTemplate.hpp"

#define FPS 165

/**
 * SDL      内部的宏定义 main   与真正的 main() 函数冲突，砍了它。
 * MingGW   内部的宏定义 __argc 与函数参数冲突，砍了它。
*/
#ifdef main
#undef main
#undef __argc
#endif

class AudioPlayBack
{
    public:
        struct TimePiece {
            Uint64 lastTime;
            Uint64 currentTime;
        };

    private:
        SystemInit::WindowSize  windowSize;
        SystemInit              sysInit;
        Audio                   audio;
        double                  audioDuration;
        TimePiece               timePiece;

        Uint64                  startTick;
        Uint64                  renderTicks;

        EventsControl           events;

        FontsTexture            musicNameShown;
        FontsTexture            musicDurationShown;
        FontsTexture            playStateShown;

        FontsTexture            fastPlayShown;
        TimePiece               fastPlayShownTimesPiece;

        void pieceTime(Uint64 __seconds);

        /**
         * @brief 根据次音频的持续时间组合出字符串，
         *        目前的格式是：xx minutes xx seconds
        */
        std::string getDurationString(double __duration);

        void handleKeyDown(const SDL_Scancode __scanCode);

        void keyControl(void);

    public:
        AudioPlayBack() : windowSize({600, 200}), sysInit(windowSize, "Audio Class Test"),
        audio(DEAULT_AUDIO_ATTRIBUTION), audioDuration(0.0), timePiece({0, 0}),
        startTick(0ULL),  renderTicks(0ULL), events(), 
        musicNameShown(), musicDurationShown(), playStateShown(), 
        fastPlayShown(), fastPlayShownTimesPiece({0, 0}) {}

        void init(void);

        void load(const char * __musicPath);

        void run(void);
};

void AudioPlayBack::pieceTime(Uint64 __seconds)
{
    if (
        ((this->timePiece.currentTime - this->timePiece.lastTime) >= (__seconds * 1000)) &&
        !this->audio.getPlayState() && this->audioDuration >= 0.0
    )
    {
        --this->audioDuration;

        this->musicDurationShown.load(
            this->getDurationString(this->audioDuration), 
            {0, 0, 0XAF, 0xFF}, sysInit.getRenderer()
        );

        this->timePiece.lastTime = this->timePiece.currentTime;
    }

    if (this->audioDuration < 0.0) 
    {
        this->playStateShown.load(
            "[STOP]", {0xFF, 0, 0, 0xFF}, 
            sysInit.getRenderer()
        );

        this->musicDurationShown.load(
            this->getDurationString(0.0), 
            {0, 0, 0XAF, 0xFF}, sysInit.getRenderer()
        );
    }
}

std::string AudioPlayBack::getDurationString(double __duration)
{
    int     minutes = __duration / 60;
    double  seconds = static_cast<int>(__duration) % 60;

    return {
        std::to_string(minutes) + " minutes " + 
        std::to_string(static_cast<int>(seconds)) + " seconds"
    };
}

void AudioPlayBack::handleKeyDown(const SDL_Scancode __scanCode)
{
    switch (__scanCode)
    {
        case SDL_SCANCODE_A:                // 暂停
            this->playStateShown.load(
                "[PAUSE]", {0xFF, 0, 0, 0xFF}, 
                this->sysInit.getRenderer()
            );

            this->audio.pause();
            break;
                
        case SDL_SCANCODE_D:                // 恢复播放
            this->playStateShown.load(
                "[PLAY]", {0, 0xFF, 0, 0xFF}, 
                this->sysInit.getRenderer()
            );

            this->audio.resume();
            break;
                
        case SDL_SCANCODE_SPACE:            // 重新播放
            this->playStateShown.load(
                "[REPLAY]", {0, 0xFF, 0, 0xFF}, 
                this->sysInit.getRenderer()
            );

            this->audio.play(0); 
            this->audioDuration = this->audio.getDuration();    
            break;

        case SDL_SCANCODE_LEFT:     // 快退 15 秒

            if (this->audio.fastRewind(15) != -1) 
            {
                this->audioDuration += 15;
                this->fastPlayShown.load(
                    "[Rewind 15 Sec]", {0, 0, 0xFF, 0xFF},
                    this->sysInit.getRenderer()
                );
                this->fastPlayShownTimesPiece.lastTime = SDL_GetTicks64();
            }
            else 
            {
                this->audioDuration = this->audio.getDuration();
                this->fastPlayShown.load(
                    "[To Begin]", {0, 0, 0xFF, 0xFF},
                    this->sysInit.getRenderer()
                );
                this->fastPlayShownTimesPiece.lastTime = SDL_GetTicks64();
            }
            break;

        case SDL_SCANCODE_RIGHT:    // 快进 15 秒
            
            if (this->audio.fastForward(15) != -1) 
            {
                this->audioDuration -= 15;
                this->fastPlayShown.load(
                    "[Forward 15 Sec]", {0, 0xFF, 0, 0xFF},
                    this->sysInit.getRenderer()
                );
                this->fastPlayShownTimesPiece.lastTime = SDL_GetTicks64();
            }
            else { 
                // 如果音乐剩余时间不足以 15 秒的快进，直接来到末尾就行。
                this->audioDuration = 0.0;
                this->fastPlayShown.load(
                    "[To End]", {0, 0xFF, 0, 0xFF},
                    this->sysInit.getRenderer()
                );
                this->fastPlayShownTimesPiece.lastTime = SDL_GetTicks64();
            }
            break;

        default:
            break;
    }
}

void AudioPlayBack::keyControl(void)
{
    if (this->events.getKeyboardState().size() == 0) { return; }

    Uint64 currentTime = SDL_GetTicks64();

    for (auto & [scanCode, keyState] : this->events.getKeyboardState())
    {
        if (keyState.pressed && !keyState.processed && (currentTime - keyState.lastKeyPressTime) > 80)
        {   
            handleKeyDown(scanCode);
            keyState.processed = true;
        }

        if (keyState.released) {}
    }
}

void AudioPlayBack::init(void) 
{
    sysInit.init();
    audio.init();

    this->startTick   = SDL_GetTicks64();
    this->renderTicks = STANDING_RENDER_TIME(this->startTick, FPS);
}

void AudioPlayBack::load(const char * __musicPath)
{
    using namespace fmt;

    this->musicNameShown.open("../fonts/Consolas-Regular.ttf", 20);
    this->musicDurationShown.open("../fonts/Consolas-Regular.ttf", 20);
    this->playStateShown.open("../fonts/Consolas-Regular.ttf", 20);
    this->fastPlayShown.open("../fonts/Consolas-Regular.ttf", 20);

    try {
        this->audio.load(__musicPath);
        this->audioDuration = this->audio.getDuration();

        this->musicNameShown.load(
            "Playing: [" + this->audio.getMusicName() + ']', 
            {0 ,0, 0, 0xFF}, this->sysInit.getRenderer()
        );

        this->playStateShown.load(
                "[PLAY]", {0, 0xFF, 0, 0xFF}, this->sysInit.getRenderer()
            );

        this->musicDurationShown.load(
            this->getDurationString(this->audioDuration), 
            {0, 0, 0XAF, 0xFF}, this->sysInit.getRenderer()
        );
    }
    catch (const std::runtime_error & __except)
    {
        print(
            fg(terminal_color::red) | emphasis::bold, 
            "{} {}\n", CurrentTime(), __except.what()
        );

        this->audio.~Audio();
        this->sysInit.~SystemInit();

        exit(EXIT_FAILURE);
    }
}

void AudioPlayBack::run(void)
{
    this->audio.play(0);

    this->timePiece.lastTime               = SDL_GetTicks64();

    while (!this->events.getRunstate())
    {
        events.recordEvents();

        SDL_RenderClear(sysInit.getRenderer());
        SDL_SetRenderDrawColor(sysInit.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);

        this->musicNameShown.render(
            windowSize.w / 2 - musicNameShown.getRenderPosition().w / 2,
            windowSize.h / 2 - musicNameShown.getRenderPosition().h / 2,
            sysInit.getRenderer()
        );

        this->musicDurationShown.render(
            windowSize.w / 2 - musicDurationShown.getRenderPosition().w / 2, 
            windowSize.h / 2 - musicDurationShown.getRenderPosition().h / 2 + 30, 
            sysInit.getRenderer()
        );

        this->playStateShown.render(0, 0, sysInit.getRenderer());

        fastPlayShownTimesPiece.currentTime = SDL_GetTicks64();
        if ((fastPlayShownTimesPiece.currentTime - fastPlayShownTimesPiece.lastTime) < 1500)
        {
            this->fastPlayShown.render(0, 25, sysInit.getRenderer());
        }

        SDL_RenderPresent(sysInit.getRenderer());

        this->timePiece.currentTime = SDL_GetTicks64();

        this->pieceTime(1);
        this->keyControl();

        frameControl(this->startTick, this->renderTicks);
    }
}

/**
 * @brief 如果用户在命令行中没有传入相应参数，要做出相应提示。
 * 
 * @param __processName 可执行文件路径（一般传入 argv[0]）
 * @param __argc        参数数量
*/
void argumentOperator(const char * __processName, const int __argc);

int main(int argc, char const * argv[])
{
    system("cls");

    argumentOperator(argv[0], argc);

    AudioPlayBack audioPlayBack;

    audioPlayBack.init();
    audioPlayBack.load(argv[1]);
    audioPlayBack.run();
    
    return EXIT_SUCCESS;
}

void argumentOperator(const char * __processName, const int __argc)
{
    using namespace fmt;

    if (__argc == 1) {
        print(
            fg(terminal_color::yellow), 
            "{} <audio file name>\n", __processName
        );

        exit(EXIT_FAILURE);
    }
}