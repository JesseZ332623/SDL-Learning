#include "../../SDLTemplateCode/include/SDLTemplate.hpp"
#include <cmath>

#define FPS 30
#define SAMPLE_COUNT 10000       // 采样点数量

class TrigonometricFunctions
{
    public:
        typedef long double (*TrigonometricFunc)(long double __x);
    private:
        SystemInit::WindowSize windowSize;
        SystemInit sysInit;

        EventsControl events;

        FontsTexture  OPointShown;

        Uint64 startTick;
        Uint64 renderTick;

        /**
         * @brief 画出直角坐标系。
        */
        void drawCoordination(void);

        /**
         * @brief 画出指定三角函数函数的一个周期。
        */
        void drawTrigonometricFunction(
            TrigonometricFunc __trigonometricFunc, double __begin, double __end, SDL_Color __color
        );

    public:
        TrigonometricFunctions(void) : 
        windowSize({1200, 700}), sysInit(windowSize, "Trigonometric Functions") {}

        void init(void);

        void load(void);

        void run(void);
};

void TrigonometricFunctions::drawCoordination(void)
{
    SDL_Color originalColor;

    SDL_GetRenderDrawColor(
        sysInit.getRenderer(), 
        &originalColor.r, &originalColor.g, &originalColor.b, &originalColor.a
    );

    SDL_SetRenderDrawColor(sysInit.getRenderer(), 0, 0, 0, 0xFF);

    /**
     * @brief 两个点确定直角坐标系的 X 轴。
    */
    SDL_Point axis_X1 = {0, windowSize.h / 2};
    SDL_Point axis_X2 = {windowSize.w, windowSize.h / 2};

    SDL_Point axis_Y1 = {windowSize.w / 2, 0};
    SDL_Point axis_Y2 = {windowSize.w / 2, windowSize.h};

    SDL_RenderDrawLine(sysInit.getRenderer(), axis_X1.x, axis_X1.y, axis_X2.x, axis_X2.y);
    SDL_RenderDrawLine(sysInit.getRenderer(), axis_Y1.x, axis_Y1.y, axis_Y2.x, axis_Y2.y);

    OPointShown.render(windowSize.w / 2 + 5, windowSize.h / 2, sysInit.getRenderer());

    SDL_SetRenderDrawColor(
        sysInit.getRenderer(), 
        originalColor.r, originalColor.g, originalColor.b, originalColor.a
    );
}

void TrigonometricFunctions::drawTrigonometricFunction(
    TrigonometricFunc __trigonometricFunc, double __begin, double __end, SDL_Color __color
)
{
    SDL_Color originalColor;

    SDL_GetRenderDrawColor(
        sysInit.getRenderer(), 
        &originalColor.r, &originalColor.g, &originalColor.b, &originalColor.a
    );

    SDL_SetRenderDrawColor(sysInit.getRenderer(), __color.r, __color.g, __color.b, __color.a);

#if false
    // [-2π，2π] 是 sin 函数的一个周期
    double sinXBegin = -(2 * M_PI);
    double sinXEnd   =  (2 * M_PI);
#endif

    double step  =  ((__end - __begin) / (windowSize.w - 1));

    for (double index = -windowSize.w; index <= windowSize.w; index += 0.1)
    {
        double x = __begin + index * step;
        double y = __trigonometricFunc(x);

        int screenX = index + windowSize.w / 2;
        int screenY = windowSize.h / 2 - (y * (windowSize.h / 2));

        if (screenX >= 0 && screenX < windowSize.w && screenY >= 0 && screenY < windowSize.h) 
        {
            SDL_RenderDrawPoint(sysInit.getRenderer(), screenX, screenY);
        }
    }

    SDL_SetRenderDrawColor(
        sysInit.getRenderer(), 
        originalColor.r, originalColor.g, originalColor.b, originalColor.a
    );
}

void TrigonometricFunctions::init(void)
{
    this->sysInit.init();

    this->startTick  = SDL_GetTicks64();
    this->renderTick = STANDING_RENDER_TIME(startTick, FPS);
}

void TrigonometricFunctions::load(void)
{
    this->OPointShown.open("../fonts/Consolas-Italic.ttf", 30);
    this->OPointShown.load("O", {0, 0, 0, 0xFF}, this->sysInit.getRenderer());
}

void TrigonometricFunctions::run(void)
{
    while (!this->events.getRunstate())
    {
        this->events.recordEvents();

        SDL_RenderClear(this->sysInit.getRenderer());
        SDL_SetRenderDrawColor(this->sysInit.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);

        this->drawCoordination();
        this->drawTrigonometricFunction(std::sin, -(2 * M_PI), (2 * M_PI), {0xFF, 0, 0, 0xFF});
        this->drawTrigonometricFunction(std::cos, -(2 * M_PI), (2 * M_PI), {0, 0xFF, 0, 0xFF});
        this->drawTrigonometricFunction(std::tan, -M_PI, M_PI, {0, 0, 0xFF, 0xFF});

        SDL_RenderPresent(this->sysInit.getRenderer());

        frameControl(this->startTick, this->renderTick);
    }
}

int WinMain(int argc, char const * argv[])
{
    system("cls");

    TrigonometricFunctions trigonometricFunctions;

    trigonometricFunctions.init();
    trigonometricFunctions.load();
    trigonometricFunctions.run();

    return EXIT_SUCCESS;
}