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
        static int KeyCount;    // SDL 能记录到的键位总数
        typedef std::unordered_map<SDL_Scancode, bool> KeyCodeMap;
    private:
        SDL_Event events;   // SDL 事件类

        bool runState;      // 程序是否运行标志位

        /**
         * @brief 一张记录键盘事件的哈希表，
         *        其中 Key 代表某个键对应 SDL 内部的键码，Value 为布尔类型，表示该键是否被按下。
        */
        KeyCodeMap keyboardState;
    
    public:
        EventsControl(void) : events(), runState(false), keyboardState() {}

        /**
         * @brief 获取程序的运行状态，确定合适退出程序。
         * 
         * @return true     收到退出信号
         * @return false    未收到退出信号
        */
        bool getRunstate(void) const { return this->runState; }

        /**
         * @brief 获取当前键盘的状态。
        */
        const KeyCodeMap & getKeyboardState(void) const { return this->keyboardState; };

        /**
         * @brief 事件记录，
         *        调用 SDL_PollEvent() 探测事件并改变相应的标志位。
        */
        void recordEvents(void);
};

#endif // __EVENTS_H__