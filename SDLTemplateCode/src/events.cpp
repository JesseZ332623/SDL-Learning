#include "../include/events.hpp"

#include "../include/fmtTime.hpp"

int EventsControl::KeyCount = SDL_NUM_SCANCODES;

void EventsControl::recordEvents(void)
{
    using namespace fmt;

#define CODE (this->events.key.keysym.scancode)

    while (SDL_PollEvent(&this->events))
    {
        switch (this->events.type)
        {
            case SDL_QUIT:
                this->runState = true;
                break;
            
            case SDL_KEYDOWN:
                if (this->keyboardState.find(CODE) != this->keyboardState.end()) {
                    this->keyboardState.at(CODE) = true;
                }
                else { this->keyboardState.insert({CODE, true}); }
                break;
            
            case SDL_KEYUP:
                this->keyboardState.at(CODE) = false;
                break;

            default:
                break;
        }
    }
}