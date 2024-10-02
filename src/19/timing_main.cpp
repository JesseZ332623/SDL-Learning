#include "./timing.h"

#ifdef main
#undef main
#endif

int main(int argc, char const *argv[])
{
    TimingShown timingShown;

    timingShown.init();
    timingShown.load();
    timingShown.run();

    return EXIT_SUCCESS;
}
