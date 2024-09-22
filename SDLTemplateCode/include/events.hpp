#ifndef __EVENTS_H__
#define __EVENTS_H__

#include "SDL_events.h"

#include <unordered_map>
#include <vector>

/**
 * @brief 比较两个坐标是否完全相等。
*/
bool operator==(const SDL_Point & __a, const SDL_Point & __b) {
    return (__a.x == __b.x) && (__a.y == __b.y);
}

/**
 * @brief 比较两个坐标是否完全不相等。
*/
bool operator!=(const SDL_Point & __a, const SDL_Point & __b) {
    return (__a.x != __b.x) && (__a.y != __b.y);
}

/**
 * @brief 判断点 A 是否在点 B 的左侧区域（包括点 B）。
*/
bool operator<=(const SDL_Point & __a, const SDL_Point & __b) {
    return (__a.x <= __b.x) && (__a.y <= __b.y);
}

/**
 * @brief 判断点 A 是否在点 B 的左侧区域（包括点 B）。
*/
bool operator>=(const SDL_Point & __a, const SDL_Point & __b) {
    return (__a.x >= __b.x) && (__a.y >= __b.y);
}

/**
 * @brief 判断点 A 是否在点 B 的左侧区域（不包括点 B）。
*/
bool operator<(const SDL_Point & __a, const SDL_Point & __b) {
    return (__a.x < __b.x) && (__a.y < __b.y);
}

/**
 * @brief 判断点 A 是否在点 B 的左侧区域（不包括点 B）。
*/
bool operator>(const SDL_Point & __a, const SDL_Point & __b) {
    return (__a.x > __b.x) && (__a.y > __b.y);
}

/**
 * @brief 比较两个坐标是否完全相等。
*/
bool operator==(const SDL_FPoint & __a, const SDL_FPoint & __b) {
    return (__a.x == __b.x) && (__a.y == __b.y);
}

/**
 * @brief 比较两个坐标是否完全不相等。
*/
bool operator!=(const SDL_FPoint & __a, const SDL_FPoint & __b) {
    return (__a.x != __b.x) && (__a.y != __b.y);
}

/**
 * @brief 判断点 A 是否在点 B 的左侧区域（包括点 B）。
*/
bool operator<=(const SDL_FPoint & __a, const SDL_FPoint & __b) {
    return (__a.x <= __b.x) && (__a.y <= __b.y);
}

/**
 * @brief 判断点 A 是否在点 B 的左侧区域（包括点 B）。
*/
bool operator>=(const SDL_FPoint & __a, const SDL_FPoint & __b) {
    return (__a.x >= __b.x) && (__a.y >= __b.y);
}

/**
 * @brief 判断点 A 是否在点 B 的左侧区域（不包括点 B）。
*/
bool operator<(const SDL_FPoint & __a, const SDL_FPoint & __b) {
    return (__a.x < __b.x) && (__a.y < __b.y);
}

/**
 * @brief 判断点 A 是否在点 B 的左侧区域（不包括点 B）。
*/
bool operator>(const SDL_FPoint & __a, const SDL_FPoint & __b) {
    return (__a.x > __b.x) && (__a.y > __b.y);
}

/**
 * @brief SDL 事件的封装类，
 *        事件操作都可以在这里扩展。
 */
class EventsControl
{
    public:
        /**
         * @brief 该结构体可以记录某个
         *        游戏控制器左右摇杆的实时位置。
        */
        struct RokerPosition 
        {
            int             deviceNo;         // 设备号
            SDL_FPoint      leftRokerPos;
            SDL_FPoint      rightRokerPos;

            std::string show(void) const 
            {   
                using std::to_string;
                return {
                    "Device No. [" + to_string(deviceNo) + "]"
                    " L Roker: (" + to_string(leftRokerPos.x) + ", " + to_string(leftRokerPos.y) + ") "
                    " R Roker: (" + to_string(rightRokerPos.x) + ", " + to_string(rightRokerPos.y) + ") "
                };
            }
        };

        static const int KeyCount;           // SDL 能记录到的键盘键位总数
        static const float RokerMotionMax;   // SDL 能记录到的摇杆移动最大值
        static const float RokerMotionMin;   // SDL 能记录到的摇杆移动最小值

        /**
         * @brief 一张记录键盘事件的哈希表，
         *        其中 Key 代表某个键对应 SDL 内部的键码，
         *        Value 为布尔类型，表示该键是否被按下。
        */
        typedef std::unordered_map<SDL_Scancode, bool> KeyCodeMap;

        /**
         * @brief 一张记录鼠标事件的哈希表，
         *        其中 Key 代表鼠标上某个键对应 SDL 内部的键码，
         *        Value 为布尔类型，表示该键是否被按下。
        */
        typedef std::unordered_map<decltype(SDL_MouseButtonEvent::button), bool> MouseButtonMap;

        /**
         * @brief 哈希表 gameControllers 用于管理多个手柄的连接与断开，
         *        其中 Key 表示的是连接本机的游戏控制器的设备号，
         *        Value 则是指向了一个游戏控制器对象的指针。
        */
        typedef std::unordered_map<int, SDL_GameController *> GameControllerMap;

        /**
         * @brief 动态数组 GameControllersPosition 用于保存
         *        多个游戏控制器左右摇杆的实时位置。
        */
        typedef std::vector<RokerPosition> GameControllerRokersPosition;

    private:
        SDL_Event           events;             // SDL 事件类

        bool                runState;           // 程序是否运行标志位

        KeyCodeMap          keyboardState;      // 键盘按键事件管理

        MouseButtonMap      mouseButtonState;   // 鼠标按键事件管理

        bool                mouseMotionState;   // 鼠标的运动状态

        SDL_Point           mousePosition;      // 记录在某一帧下鼠标位于窗口的坐标。

        GameControllerMap            gameControllers;      // 游戏控制器资源管理

        GameControllerRokersPosition rokersPosition;       // 游戏控制器左右摇杆记录

        /**
         * @brief 当键盘的某一个键按下时要做的操作。 
        */
        void keyPress(void);

        /**
         * @brief 当键盘的某一个键回弹时要做的操作。 
        */
        void keyRelease(void);

        /**
         * @brief 当鼠标的某一个键按下时要做的操作。 
        */
        void mouseButtonPress(void);

        /**
         * @brief 当鼠标的某一个键松开时要做的操作。 
        */
        void mouseButtonRelease(void);

        /**
         * @brief 当鼠标在移动时要做的操作。
        */
        void mouseMotion(void);

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

        /**
         * @brief 有些时候断开手柄并不能触发 SDL_CONTROLLERDEVICEREMOVED 信号，
         *        所以需要一个函数来手动检测控制器连接状态，以正确处理相关资源。
        */
        //void checkControllerStatus(void);

        /**
         * @brief 当某一个游戏控制器连接要做的操作。
        */
        void gameControllerAdded(void);

        /**
         * @brief 当某一个游戏控制器断开要做的操作。
        */
        void gameControllerMoved(void);

        /**
         * @brief 处理游戏控制器的左右摇杆运动。
        */
        void processGameControllerRokerMotion(void);
    
    public:
        EventsControl(void) : 
        events(), runState(false), keyboardState(), 
        mouseButtonState(), mouseMotionState(false), mousePosition(),
        gameControllers(), rokersPosition() {}

        /**
         * @brief 获取程序的运行状态，确定合适的时机来退出程序。
         * 
         * @return true     收到退出信号
         * @return false    未收到退出信号
        */
        bool getRunstate(void) const { 
            return this->runState; 
        }

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
         * @brief 获取所有连接系统的游戏控制器的左右摇杆位置。
        */
        const GameControllerRokersPosition & getRockersPosition(void) const {
            return this->rokersPosition;
        }

        /**
         * @brief 事件记录，
         *        调用 SDL_PollEvent() 探测事件并改变相应的标志位。
        */
        void recordEvents(void);

        /**
         * @brief 类析构的时候需要检查 GameControllerMap 内是否还有成员，
         *        如果有，需要处理掉避免泄漏。
        */
        ~EventsControl();
};

#endif // __EVENTS_H__