#include "../include/frameControl.hpp"

#include "SDL_timer.h"

void frameControl(Uint64 & __startFrameTick, const Uint64 __frameRenderTime)
{

    Uint64 currentTick = SDL_GetTicks64();
    Uint64 renderTime  = currentTick - __startFrameTick;

    if (renderTime < __frameRenderTime) { 
        SDL_Delay(__frameRenderTime - renderTime); 
    }

    __startFrameTick = SDL_GetTicks64();
}