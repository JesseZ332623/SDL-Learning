#include "./timer.hpp"

#undef main
#undef __argc
#undef __argv

/**
 * @brief 对单个时间参数进行检查，然后在拼合进时间字符串中。
*/
void timeCheck(const char * __time, std::string & __initTime);

/**
 * @brief 检查输入的命令行参数，如果检查通过。
 *        就拼合一个时间字符串并返回。
*/
std::string argumentsCheck(const int __argc, char const * __argv[]);

int main(int argc, char const *argv[])
{
    std::string initTime = argumentsCheck(argc, argv);

    Timer timer;

    timer.init();
    timer.load(initTime);
    timer.run();

    return EXIT_SUCCESS;
}

void timeCheck(const char * __time, std::string & __initTime)
{
    using std::stoi;
    using namespace fmt;

    int timeValue = 0;

    try {
        timeValue = stoi(__time);
    }
    catch (const std::invalid_argument & __excpt)
    {
        print(
            fg(terminal_color::red) | emphasis::bold, 
            "timeCheck(): {}.\n", __excpt.what()
        );
        std::exit(EXIT_FAILURE);
    }

    if (timeValue >= 10 && timeValue < 100) 
    {
        __initTime += __time;
        __initTime += " : ";
    }
    else if (timeValue < 10) 
    {
        __initTime += '0';
        __initTime += __time;
        __initTime += " : ";
    }
}

std::string argumentsCheck(const int __argc, char const * __argv[])
{
    using namespace fmt;

    system("cls");

    std::string optionInfo = format("{} <hours> <minuts> <seconds>\n", __argv[0]);
    std::string initTime;

    if (__argc < 4) 
    {
        print(fg(terminal_color::red) | emphasis::bold, "{}", optionInfo);
        std::exit(EXIT_FAILURE);
    }
    else
    {
        for (int index = 1; index < 4; ++index) {
            timeCheck(__argv[index], initTime);
        }

        initTime.erase(initTime.end() - 3, initTime.end());
    }

    return initTime;
}