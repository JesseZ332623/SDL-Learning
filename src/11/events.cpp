#include "../../include/11/events.hpp"

void EventsControl::eventsRecord(void)
{
    while (SDL_PollEvent(&this->events))
    {
        if (this->events.type == SDL_QUIT) {
            this->runState = true;
        }
    }
}