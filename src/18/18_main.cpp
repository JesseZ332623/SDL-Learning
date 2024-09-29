#include "../../SDLTemplateCode/include/SDLTemplate.hpp"

#include <thread>
#include <iostream>

#if false
#include <mutex>                // 互斥锁
#include <condition_variable>   // 条件变量
#endif

#ifdef main
#undef main
#undef __argc
#endif

#define FPS 165

class ThreadTextShown
{
    private:
        SystemInit::WindowSize windowSize;
        SystemInit      sysInit;

        FontsTexture    text;

        EventsControl   events;

        std::string     renderContent;

        Uint64       startTick;
        Uint64       renderTick;

    public:
        ThreadTextShown(void) : 
        windowSize({800, 800}), sysInit(windowSize, "18. Thread Render Control"),
        renderContent("None") {}

        void changeRenderText(void);

        void setRenderContent(std::string __content) {
            this->renderContent = __content;
        }

        void init(void);

        void load(void);

        void run(void);
};

void ThreadTextShown::changeRenderText(void)
{
    using namespace fmt;

    std::string tempString;

    while (true)
    {
        print(
            fg(terminal_color::bright_blue), "Enter something to render: \n"
        );

        std::getline(std::cin, tempString);

        if (tempString.size()) {
            this->setRenderContent(tempString);
        }
        else {
            print(
                fg(terminal_color::red) | emphasis::italic, 
                "Don't pass empty string.\n"
            );
        }
    }
}

void ThreadTextShown::init(void)
{
    sysInit.init();

    this->startTick = SDL_GetTicks64();
    this->renderTick = STANDING_RENDER_TIME(this->startTick, FPS);
}

void ThreadTextShown::load(void)
{
    this->text.open("../fonts/Consolas-Regular.ttf", 45);
    this->text.load(this->renderContent, {0, 0, 0, 0xFF}, this->sysInit.getRenderer());
}

void ThreadTextShown::run(void)
{
    while (!this->events.getRunstate())
    {
        this->events.recordEvents();

        /**
         * @brief 这里如果不考虑使用互斥锁（mutex）和
         *        条件变量（condition_variable）加以限制，很多 load 的过程都是浪费的。。。。
        */
        this->text.load(this->renderContent, {0, 0, 0, 0xFF}, this->sysInit.getRenderer());
        SDL_Rect textRenderPos = this->text.getRenderPosition();

        SDL_RenderClear(this->sysInit.getRenderer());
        SDL_SetRenderDrawColor(
            this->sysInit.getRenderer(), 
            0xFF, 0xFF, 0xFF, 0xFF
        );

        this->text.render(
            this->windowSize.w / 2 - textRenderPos.w / 2,
            this->windowSize.h / 2 - textRenderPos.h / 2,
            this->sysInit.getRenderer()
        );

        SDL_RenderPresent(this->sysInit.getRenderer());

        frameControl(this->startTick, this->renderTick);
    }
}

int main(int argc, char const *argv[])
{
    system("cls");

    ThreadTextShown threadTextShown;

    threadTextShown.init();
    threadTextShown.load();

    std::thread thread(&ThreadTextShown::changeRenderText, &threadTextShown);
    thread.detach();

    threadTextShown.run();

    return EXIT_SUCCESS;
}
