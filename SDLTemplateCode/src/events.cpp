#include "../include/events.hpp"

#include "../include/fmtTime.hpp"

#include "SDL_timer.h"

int EventsControl::KeyCount = SDL_NUM_SCANCODES;

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

void EventsControl::recordEvents(void)
{
    using namespace fmt;

/**
 * @brief 获取键盘中某个键按下或松开时，该键对应的键码。
*/
#define GET_KEYCODE       (this->events.key.keysym.scancode)

/**
 * @brief 获取鼠标中某个键按下或松开时，该键对应的键码。
*/
#define GET_MOUSE_KEYCODE (this->events.button.button)

#define IF_KEYMAP_FOUND (this->keyboardState.find(GET_KEYCODE) != this->keyboardState.end())

#define IF_MOUSEMAP_FOUND (this->mouseButtonState.find(GET_MOUSE_KEYCODE) != this->mouseButtonState.end())

    while (SDL_PollEvent(&this->events))
    {
        switch (this->events.type)
        {
            case SDL_QUIT:
                this->runState = true;
                break;
            
            // 记录键盘中某个键被按下
            case SDL_KEYDOWN:
                if (IF_KEYMAP_FOUND) {
                    this->keyboardState[GET_KEYCODE] = true;
                }
                else { this->keyboardState.insert({GET_KEYCODE, true}); }
                break;

            // 记录键盘中某个键回弹
            case SDL_KEYUP:
                this->keyboardState[GET_KEYCODE] = false;
                break;

            // 记录鼠标的某个按键按下
            case SDL_MOUSEBUTTONDOWN:
                if (IF_MOUSEMAP_FOUND) {
                    this->mouseButtonState[GET_MOUSE_KEYCODE] = true;
                }
                else { this->mouseButtonState.insert({GET_MOUSE_KEYCODE, true}); }
                break;
#if false       
                switch (this->events.button.button)
                {
                    case SDL_BUTTON_LEFT:   // 鼠标左键按下
                        break;
                    case SDL_BUTTON_RIGHT:  // 鼠标右键按下
                        break;
                    case SDL_BUTTON_MIDDLE: // 鼠标中键按下
                        break;
                    case SDL_BUTTON_X1:     // 鼠标侧键 1 按下
                        break;
                    case SDL_BUTTON_X2:     // 鼠标侧键 2 按下
                        break;
                    /**
                     * 鼠标未知键位按下（像高级的鼠标会有很多的侧键，SDL 对它们似乎没做映射），
                     * 当然大部分情况也用不到不是吗（喜）
                    */
                    default:    
                        break;
                }
#endif      
            // 记录鼠标的某个按键回弹
            case SDL_MOUSEBUTTONUP:
                this->mouseButtonState[GET_MOUSE_KEYCODE] = false;
                break;
#if false
                switch (this->events.button.button)
                {
                    case SDL_BUTTON_LEFT:   // 鼠标左键回弹
                        break;
                    case SDL_BUTTON_RIGHT:  // 鼠标右键回弹
                        break;
                    case SDL_BUTTON_MIDDLE: // 鼠标中键回弹
                        break;
                    case SDL_BUTTON_X1:     // 鼠标侧键 1 回弹
                        break;
                    case SDL_BUTTON_X2:     // 鼠标侧键 2 回弹
                        break;
                    default:    
                        break;
                }
#endif
            /** 
             * 记录鼠标滚轮的运动（一般的鼠标仅支持滚轮的垂直滚动），
             * 但有一些专业鼠标支持水平滚动（比如 罗技 MX Master 3S，它的水平滚轮在侧面），
             * 所以此处忽略鼠标滚轮水平滚动的事件。
             * 
             * （由于项目目前用不到鼠标滚轮，这部分暂时不扩展）
            */
            case SDL_MOUSEWHEEL:
                // 鼠标滚轮向上滚动
                if (this->events.wheel.y > 0) {
                    
                // 鼠标滚轮向下滚动
                } else if (this->events.wheel.y < 0) {

                }
                break;
            
            // 记录鼠标的运动（把坐标赋值并改变标志位即可）
            case SDL_MOUSEMOTION:

                mouseMotionState = true;

                this->mousePosition.x = this->events.motion.x;
                this->mousePosition.y = this->events.motion.y;
                break;

            default:
                break;
        }

        if (!isMouseMotionless()) { 
            mouseMotionState = false;
        }
    }
}