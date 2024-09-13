#include "../include/fmtTime.hpp"

std::string CurrentTime(void)
{
    using namespace fmt;
    using namespace std::chrono;

    // 获取当前时间（东八区）
    auto now                = system_clock::now() + hours(8);
    // 转换成时间戳
    auto durationEpoch      = now.time_since_epoch();
    // 提取毫秒数
    auto millisecondsSinceEpoch = duration_cast<milliseconds>(durationEpoch);

    /**
     * @brief 返回格式化后的字符串（年-月-日 时:分:秒.毫秒） 
    */
    return format(
        "[{:%Y-%m-%d} {}]", 
        now, fmt::format("{:%T}", millisecondsSinceEpoch)
    );
}