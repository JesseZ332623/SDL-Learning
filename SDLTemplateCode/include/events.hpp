#ifndef __EVENTS_H__
#define __EVENTS_H__

#include "SDL_events.h"

#include <unordered_map>

/**
 * @brief SDL 事件的封装类，
 *        事件操作都可以在这里扩展。
 */
class EventsControl
{
    public:
        static int KeyCount;    // SDL 能记录到的键盘键位总数
        typedef std::unordered_map<SDL_Scancode, bool> KeyCodeMap;
        typedef std::unordered_map<decltype(SDL_MouseButtonEvent::button), bool> MouseButtonMap;
    private:
        SDL_Event events;   // SDL 事件类

        bool runState;      // 程序是否运行标志位

        /**
         * @brief 一张记录键盘事件的哈希表，
         *        其中 Key 代表某个键对应 SDL 内部的键码，
         *        Value 为布尔类型，表示该键是否被按下。
        */
        KeyCodeMap keyboardState;

        /**
         * @brief 一张记录鼠标事件的哈希表，
         *        其中 Key 代表鼠标上某个键对应 SDL 内部的键码，
         *        Value 为布尔类型，表示该键是否被按下。
        */
        MouseButtonMap mouseButtonState;

        bool mouseMotionState;  // 鼠标的运动状态

        /**
         * @brief 记录在某一帧下鼠标位于窗口的坐标。
        */
        SDL_Point mousePosition;

        /**
         * @brief 每隔一小段时间检查鼠标的位置，由此判断鼠标是否停止。
         * 
         * @param __interval 每隔多少毫秒检测一次（默认 100 毫秒）
         * @param __times    检测次数（默认 3 次）
         * 
         * @return true  鼠标已静止
         * @return false 鼠标在移动
        */
        bool isMouseMotionless(const Uint32 __interval = 100, int __times = 3);
    
    public:
        EventsControl(void) : 
        events(), runState(false), keyboardState(), 
        mouseButtonState(), mouseMotionState(false), mousePosition() {}

        /**
         * @brief 获取程序的运行状态，确定合适的时机来退出程序。
         * 
         * @return true     收到退出信号
         * @return false    未收到退出信号
        */
        bool getRunstate(void) const { return this->runState; }

        /**
         * @brief 获取当前键盘键位的状态。
        */
        const KeyCodeMap & getKeyboardState(void) const { 
            return this->keyboardState; 
        };

        /**
         * @brief 获取当前鼠标键位的状态。
        */
        const MouseButtonMap & getMouseState(void) const { 
            return this->mouseButtonState; 
        }

        /**
         * @brief 获取鼠标的运动状态
         * 
         * @return true  鼠标已静止
         * @return false 鼠标在移动
        */
        bool getMouseMotionState(void) const {
            return this->mouseMotionState;
        }

        /**
         * @brief 获取当前的鼠标位置。
        */
        const SDL_Point getMousePosition(void) const { 
            return this->mousePosition; 
        }

        /**
         * @brief 事件记录，
         *        调用 SDL_PollEvent() 探测事件并改变相应的标志位。
        */
        void recordEvents(void);
};

#endif // __EVENTS_H__