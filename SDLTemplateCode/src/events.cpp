#include "../include/events.hpp"

void EventsControl::recordEvents(void)
{
    while (SDL_PollEvent(&this->events))
    {
        if (this->events.type == SDL_QUIT) {
            this->runState = true;
        }
    }
}