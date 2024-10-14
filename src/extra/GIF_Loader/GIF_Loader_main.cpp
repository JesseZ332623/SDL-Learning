#include "./GIF_Loader.hpp"

#ifdef main
#undef main
#undef __argc
#undef __argv
#endif

void argOperator(const int __argc, char const *__argv[])
{
    system("cls");

    if (__argc != 2) 
    { 
        fmt::print(fmt::fg(fmt::terminal_color::red), "{} <GIF Path>\n", __argv[0]);
        exit(EXIT_FAILURE); 
    }
}

int main(int argc, char const *argv[])
{
    argOperator(argc, argv);

    GIFLoader Loader;

    Loader.init();
    Loader.load(argv[1]);
    Loader.run();

    return EXIT_SUCCESS;
}
