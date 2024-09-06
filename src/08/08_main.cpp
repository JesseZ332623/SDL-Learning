#include "../../include/08.hpp"

#include <unordered_map>

/**
 * @brief 根据键盘事件哈希表 __keyState 
 *        的记录来改变图像的渲染位置，纹理的裁剪位置等。
 * 
 * @param __keyState    键盘事件哈希表
 * @param __texturePos  纹理相对于屏幕的渲染位置信息
 * @param __clipPos     相对于整个纹理的裁剪位置信息
 * @param __speed       移速
*/
void moveOperator(
    const std::vector<bool> & __keyState, 
    SDL_Rect & __texturePos, SDL_Rect & __clipPos, int __speed
);

/**
 * @brief 帧率的控制
 * 
 * @param __frameStart  某一帧开始渲染的时间刻
 * @param __frameTime   维持指定帧数渲染一帧所用的时间
*/
void frameControl(Uint64 & __frameStart, const Uint64 __frameTime);

/**
 * @brief 纹理的渲染位置结构体。 
*/
struct Position { int x; int y; };

/**
 * @brief 求平面直角坐标系中 a, b 两点的距离。
*/
Uint32 getPointDistance(const Position __a, const Position __b);

/**
 * @brief 随机在屏幕内生成分散不重复的点，存储在 __recpos 哈希表中。 
*/
void assignPosition(std::unordered_map<int, Position> & __recpos, RandomGenerater & __random);

int WinMain(int argc, char const *argv[])
{
    using namespace MyLib::MyLoger;

    system("cls");

    SystemInit      sysInit;            // 初始化所有 SDL 组件类
    RandomGenerater randomGenerater;    

    Texture       circles; // 图片纹理

    std::vector<Texture>               rectangles(BARRIER_COUNT);    // 多个障碍物纹理
    std::unordered_map<int, Position>  rectanglesPos;                // 障碍物的生成位置（不重复）

    EventsControl eventsControl;        // 事件控制类

    sysInit.init(); // 初始化所有 SDL 组件

    Uint64 frameStartTick = SDL_GetTicks64();
    Uint64 farameTime     = frameStartTick / FPS;

    circles.loadFromFile("../img/08/circles.png", sysInit.getRenderer());

    for (int count = 0; count < BARRIER_COUNT; ++count)
    {
        rectangles[count].loadRectangle(BARRIER_SIZE, BARRIER_SIZE, sysInit.getRenderer());
    }
    assignPosition(rectanglesPos, randomGenerater);
    
    SDL_Rect originalPos = {0};     // 图片在屏幕的初始位置和长宽

    // 图片纹理的初始裁剪位置和长宽
    SDL_Rect clipPos = {0, 0, CIRCLE_DIAMETER, CIRCLE_DIAMETER};

#if true
    while (!eventsControl.getRunState())
    {
        eventsControl.events();
        moveOperator(eventsControl.getEventsRecord(), originalPos, clipPos, SPEED);

        SDL_SetRenderDrawColor(sysInit.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(sysInit.getRenderer());

        for (int count = 0; count < BARRIER_COUNT; ++count) { 
           rectangles.at(count).render(
                                rectanglesPos.at(count).x, rectanglesPos.at(count).y, 
                                BARRIER_COLOR, sysInit.getRenderer()
                            );
        }

        circles.render(
            originalPos.x, originalPos.y, 
            clipPos, 
            sysInit.getRenderer()
        );

        SDL_RenderPresent(sysInit.getRenderer());

        frameControl(frameStartTick, farameTime);
    }
#endif

    return EXIT_SUCCESS;
}

void moveOperator(
    const std::vector<bool> & __keyState, 
    SDL_Rect & __texturePos, SDL_Rect & __clipPos, int __speed
) {
    if (__keyState[SDL_SCANCODE_W]) { 
        __clipPos.x = __clipPos.y = 0;
        __texturePos.y -= __speed; 
    }
    else if (__keyState[SDL_SCANCODE_A]) {
        __clipPos.x = CIRCLE_DIAMETER;
        __clipPos.y = 0;
        __texturePos.x -= __speed; 
    }
    else if (__keyState[SDL_SCANCODE_S]) { 
        __clipPos.x = 0;
        __clipPos.y = CIRCLE_DIAMETER;
        __texturePos.y += __speed; 
    }
    else if (__keyState[SDL_SCANCODE_D]) { 
        __clipPos.x = __clipPos.y = CIRCLE_DIAMETER;
        __texturePos.x += __speed; 
    }
}

void frameControl(Uint64 & __frameStart, const Uint64 __frameTime)
{
    Uint64 currentTick = SDL_GetTicks64();              // 当前时间刻

    /**
     * @brief 从这一帧渲染开始的时间，到渲染完成后所用的时间
    */
    Uint64 passedTick  = currentTick - __frameStart;

    /**
     * @brief 如果这一帧渲染过快（小于标准的一帧渲染时间）
    */
    if (passedTick < __frameTime) { 
        // 就需要暂停程序，补上这一点时间
        SDL_Delay(__frameTime - passedTick); 
    }

    // 更新某一帧开始渲染的时间刻
    __frameStart = SDL_GetTicks64();
}

Uint32 getPointDistance(const Position __a, const Position __b)
{
    Uint32 dx = std::abs(__b.x - __a.x);
    Uint32 dy = std::abs(__b.y - __a.y);

    return sqrt(dx * dx + dy * dy);
}

void assignPosition(std::unordered_map<int, Position> & __recpos, RandomGenerater & __random)
{
    Position tempPos = {
        __random.getRandNum(BARRIER_SIZE, SCREEN_WIDTH - BARRIER_SIZE),
        __random.getRandNum(BARRIER_SIZE, SCREEN_HEIGHT - BARRIER_SIZE)
    };
    __recpos.insert({0, tempPos});

    for (int count = 1; count < BARRIER_COUNT; ++count)
    {
        tempPos = {
            __random.getRandNum(BARRIER_SIZE, SCREEN_WIDTH - BARRIER_SIZE),
            __random.getRandNum(BARRIER_SIZE, SCREEN_HEIGHT - BARRIER_SIZE)
        };

        for (int poll = 0; poll < count; ++poll)
        {
            if (getPointDistance(__recpos[poll], tempPos) <= BARRIER_MAX_DISTANCE)
            {
                --count;
                break;
            }
        }

        __recpos.insert({count, tempPos});
#if true
        using namespace MyLib::MyLoger;
        NOTIFY_LOG(
            "Barrier No. " + std::to_string(count) + 
            " Position (x, y) = (" + 
            std::to_string(__recpos[count].x) + ", " + std::to_string(__recpos[count].y) + ")\n"
        );
#endif
    }
}