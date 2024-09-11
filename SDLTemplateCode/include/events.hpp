#include "SDL_events.h"

class EventsControl
{
    private:
        SDL_Event events;
        bool runState;
    
    public:
        EventsControl(void) : events(), runState(false) {}

        bool getRunstate(void) const { return this->runState; }

        void eventsRecord(void);
};