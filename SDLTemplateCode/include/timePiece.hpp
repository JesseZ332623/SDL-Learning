#ifndef __TIME_PIECE_H__
#define __TIME_PIECE_H__

#include "SDL_timer.h"
#include "fmt/core.h"

/**
 * @brief 一个精巧的计时器类，
 *        很多定时执行的操作依赖它完成。 
*/
class TimePiece
{
    private:
        Uint64 last;
        Uint64 current;

        void updateTick(void) {
            this->last = this->current;
        }
    
    public:
        TimePiece(void) : last(0), current(0) {}

        /**
         * @brief 调用 SDL_GetTicks64() 保存上一个时间刻。
         *        通常在 SDL 主循环开始前要调用一次。
        */
        void setLast(void) {
            this->last = SDL_GetTicks64();
        }

        /**
         * @brief 调用 SDL_GetTicks64() 保存当前时间刻。
        */
        void setCurrent(void) {
            this->current = SDL_GetTicks64();
        }

        /**
         * @brief 显示当前的 last 和 current 时间刻（调试时使用）。 
        */
        void shown(void) const;

        /**
         * @brief 在 __intervalTime 毫秒内执行一次 __op 操作
         * 
         * @tparam Operation        执行的操作类型，可以是函数指针，仿函数，Lamba 表达式类型
         * 
         * @param __intervalTime    间隔时间（ms）
         * @param __op              执行的操作
        */
        template <typename Operation>
        void execute(Uint64 __intervalTime, Operation __op);
};

void TimePiece::shown(void) const 
{
    fmt::print(
        "Last tick: {}, Current tick: {}.\n", this->last, this->current
    );
}

template <typename Operation>
void TimePiece::execute(Uint64 __intervalTime, Operation __op)
{
    if (__intervalTime >= 0 && (this->current - this->last) >= __intervalTime) 
    {
        __op();

        this->updateTick();
    }
}

#endif  // __TIME_PIECE_H__