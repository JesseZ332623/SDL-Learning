#include "../include/events.hpp"

#include "../include/fmtTime.hpp"

#include "SDL_timer.h"
#include <stdexcept>

const int EventsControl::KeyCount = SDL_NUM_SCANCODES;
const float EventsControl::RokerMotionMax = 32767.0F;
const float EventsControl::RokerMotionMin = -32767.0F;

bool EventsControl::isMouseMotionless(const Uint32 interval, int times)
{
    static Uint32       lastCheckTime = 0;
    static SDL_Point    tempMousePos;
    static int          counter = 0;

    Uint32 currentTime = SDL_GetTicks();

    if (currentTime - lastCheckTime >= interval) {
        SDL_GetMouseState(&tempMousePos.x, &tempMousePos.y);
        lastCheckTime = currentTime;

        // 重置计数器
        if (counter == 0) { counter = times; }

        // 检查鼠标位置
        if (tempMousePos.x == this->mousePosition.x && tempMousePos.y == this->mousePosition.y) 
        {
            --counter;
            if (counter <= 0) { return true; }
            } else {
                counter = times;  // 位置变化，重置计数器
            }
        }

    return false;
}

/**
 * @brief 获取键盘中某个键按下或松开时，该键对应的键码。
*/
#define GET_KEYCODE (this->events.key.keysym.scancode)

void EventsControl::keyPress(void)
{
/**
 * @brief 某个键是否已经按过。
*/
#define IF_KEYMAP_FOUND (                                                               \
    (targetIter = this->keyboardState.find(GET_KEYCODE)) != this->keyboardState.end()   \
)
    KeyCodeMap::iterator targetIter;

    if (IF_KEYMAP_FOUND) {
        targetIter->second = true;
    }
    else { 
        this->keyboardState.insert({GET_KEYCODE, true});
    }
}

void EventsControl::keyRelease(void)
{
    this->keyboardState[GET_KEYCODE] = false;
}

/**
 * @brief 获取鼠标中某个键按下或松开时，该键对应的键码。
*/
#define GET_MOUSE_KEYCODE (this->events.button.button)

void EventsControl::mouseButtonPress(void)
{
#define IF_MOUSEMAP_FOUND (                                                                        \
    (targetIter = this->mouseButtonState.find(GET_MOUSE_KEYCODE)) != this->mouseButtonState.end()  \
)
    MouseButtonMap::iterator targetIter;

    if (IF_MOUSEMAP_FOUND) {
        targetIter->second = true;
    }
    else { this->mouseButtonState.insert({GET_MOUSE_KEYCODE, true}); }
}

void EventsControl::mouseButtonRelease(void)
{
    this->mouseButtonState[GET_MOUSE_KEYCODE] = false;
}

void EventsControl::gameControllerAdded(void)
{
    using namespace fmt;

    // 获取设备号
    Sint32 deviceIndex = this->events.cdevice.which;

    if (SDL_IsGameController(deviceIndex))
    {
        SDL_GameController * tempGameController = SDL_GameControllerOpen(deviceIndex);

        if (!tempGameController) {
            throw std::runtime_error(
                "Failed to Open this game controller, index = " + std::to_string(deviceIndex) + 
                " SDL ERROR: " + std::string(SDL_GetError()) + '\n'
            );
        }
        else 
        {
            this->gameControllers.insert({deviceIndex, tempGameController});
            this->rokersPosition.push_back({deviceIndex, {0.0F, 0.0F}, {0.0F, 0.0F}});

            print(
                fg(terminal_color::bright_green),
                "{} Game controller: [{}] connected.\n", 
                CurrentTime(), deviceIndex
            );
        }   
    }
}

void EventsControl::gameControllerMoved(void)
{
    using namespace fmt;

#define IF_GAMECONTROLLERMAP_FOUND (                                        \
    (targetIter = this->gameControllers.find(deviceIndex)) != this->gameControllers.end()  \
)

    // 获取设备号
    Sint32 deviceIndex = this->events.cdevice.which;
    GameControllerMap::iterator targetIter;
    
    if (IF_GAMECONTROLLERMAP_FOUND) 
    {
        SDL_GameControllerClose(targetIter->second);
        this->gameControllers.erase(targetIter);

        auto iter = this->rokersPosition.begin();
        while (iter != this->rokersPosition.end())
        {
            if (iter->deviceNo == deviceIndex) {
                iter = this->rokersPosition.erase(iter);
            } else {
                ++iter;
            }
        }

        printf("rokersPosition vector size = %zd\n", this->rokersPosition.size());

        if (this->rokersPosition.size() == 0) { this->rokersPosition.clear(); }

        print(
                fg(terminal_color::bright_yellow),
                "{} Game controller: [{}] disconnected.\n", 
                CurrentTime(), deviceIndex
        );
    }
}

/**
 * 设置摇杆阈值，
 * 摇杆坐标在上下左右 THRESHOLD 范围内都被视为摇杆回中。
 */
#define THRESHOLD 500

void EventsControl::processGameControllerRokerMotion(void)
{
    using std::abs;

    for (RokerPosition & rokerPos : this->rokersPosition)
    {
        Sint16 leftX = SDL_GameControllerGetAxis(
                                        this->gameControllers[rokerPos.deviceNo], 
                                        SDL_CONTROLLER_AXIS_LEFTX
                                    );
        Sint16 leftY = SDL_GameControllerGetAxis(
                                        this->gameControllers[rokerPos.deviceNo], 
                                        SDL_CONTROLLER_AXIS_LEFTY
                                    );
        Sint16 rightX = SDL_GameControllerGetAxis(
                                        this->gameControllers[rokerPos.deviceNo],
                                        SDL_CONTROLLER_AXIS_RIGHTX
                                    );
        Sint16 rightY = SDL_GameControllerGetAxis(
                                        this->gameControllers[rokerPos.deviceNo],
                                        SDL_CONTROLLER_AXIS_RIGHTY
                                    );
        
        // 检查左摇杆是否回中
        bool leftRokerCentered = (abs(leftX) < THRESHOLD) && (abs(leftY) < THRESHOLD);

        // 检查右摇杆是否回中
        bool rightRokerCentered = (abs(rightX) < THRESHOLD) && (abs(rightY) < THRESHOLD);

        // 如果左摇杆未回中，则更新左摇杆坐标
        if (!leftRokerCentered)
        {
            rokerPos.leftRokerPos.x = static_cast<float>(leftX) / this->RokerMotionMax;
            rokerPos.leftRokerPos.y = static_cast<float>(leftY) / this->RokerMotionMax;
        }
        else 
        {
            rokerPos.leftRokerPos.x = 0.0F;
            rokerPos.leftRokerPos.y = 0.0F;
        }

        // 如果右摇杆未回中，则更新右摇杆坐标
        if (!rightRokerCentered)
        {
            rokerPos.rightRokerPos.x = static_cast<float>(rightX) / this->RokerMotionMax;
            rokerPos.rightRokerPos.y = static_cast<float>(rightY) / this->RokerMotionMax;
        }
        else
        {
            rokerPos.rightRokerPos.x = 0.0F;
            rokerPos.rightRokerPos.y = 0.0F;
        }
    }
}

void EventsControl::mouseMotion(void)
{
    mouseMotionState = true;

    this->mousePosition.x = this->events.motion.x;
    this->mousePosition.y = this->events.motion.y;
}

void EventsControl::recordEvents(void)
{
    using namespace fmt;

    while (SDL_PollEvent(&this->events))
    {
        switch (this->events.type)
        {
            case SDL_QUIT:
                this->runState = true;
                break;
            
            // 记录键盘中某个键被按下
            case SDL_KEYDOWN:
                this->keyPress();
                break;

            // 记录键盘中某个键回弹
            case SDL_KEYUP:
                this->keyRelease();
                break;

            // 记录鼠标的某个按键按下
            case SDL_MOUSEBUTTONDOWN:
                this->mouseButtonPress();
                break;

            // 记录鼠标的某个按键回弹
            case SDL_MOUSEBUTTONUP:
                this->mouseButtonRelease();
                break;
#if false
            /** 
             * 记录鼠标滚轮的运动（一般的鼠标仅支持滚轮的垂直滚动），
             * 但有一些专业鼠标支持水平滚动（比如 罗技 MX Master 3S，它的水平滚轮在侧面），
             * 所以此处忽略鼠标滚轮水平滚动的事件。
             * 
             * （由于项目目前用不到鼠标滚轮，这部分暂时不扩展）
            */
            case SDL_MOUSEWHEEL:
                // 鼠标滚轮向上滚动
                if (this->events.wheel.y > 0) {}
                    
                // 鼠标滚轮向下滚动
                else if (this->events.wheel.y < 0) {}
                break;
#endif
            
            // 记录鼠标的运动（把坐标赋值并改变标志位即可）
            case SDL_MOUSEMOTION:
                this->mouseMotion();
                break;

            // 记录游戏控制器是否连接
            case SDL_CONTROLLERDEVICEADDED:
                this->gameControllerAdded();
                break;

            // 记录游戏控制器是否断开
            case SDL_CONTROLLERDEVICEREMOVED:
                this->gameControllerMoved();
                break;

            // 记录游戏控制器左右摇杆的运动
            case SDL_CONTROLLERAXISMOTION:
                this->processGameControllerRokerMotion();
                break;
                
            default:
                break;
        }

        if (!this->mouseMotionState && !this->isMouseMotionless()) { 
            mouseMotionState = false;
        }
    }
}

EventsControl::~EventsControl()
{
    for (auto pair : this->gameControllers) {
        SDL_GameControllerClose(pair.second);
    }
    
    this->gameControllers.clear();
}