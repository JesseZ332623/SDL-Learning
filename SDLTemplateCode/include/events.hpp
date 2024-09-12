#include "SDL_events.h"

/**
 * @brief SDL 事件的封装类，
 *        事件操作都可以在这里扩展。
 */
class EventsControl
{
    private:
        SDL_Event events;   // SDL 事件类
        bool runState;      // 程序是否运行标志位
    
    public:
        EventsControl(void) : events(), runState(false) {}

        /**
         * @brief 获取程序的运行状态，确定合适退出程序。
         * 
         * @return true     收到退出信号
         * @return false    未收到退出信号
        */
        bool getRunstate(void) const { return this->runState; }

        /**
         * @brief 事件记录，
         *        调用 SDL_PollEvent() 探测事件并改变相应的标志位。
        */
        void recordEvents(void);
};