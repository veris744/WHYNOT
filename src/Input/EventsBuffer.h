#pragma once
#include <deque>
#include <stdexcept>

enum class EventType {
    KeyPress,
    KeyRelease,
    MouseButtonPress,
    MouseButtonRelease,
    MouseMove,
    MouseScroll
};

struct InputEvent
{
    EventType type;
    int key;
    int action;     // GLFW_PRESS, GLFW_RELEASE, etc.
    double x, y;
    int mods; 
};

class EventsBuffer
{
    std::deque<InputEvent> events;
    
public:

    void AddEvent(InputEvent event)
    {
        events.push_back(event);
    }
    
    InputEvent PopEvent() {
        if (events.empty()) {
            throw std::out_of_range("No events in buffer");
        }
        InputEvent event = events.front();
        events.pop_front();
        return event;
    }

    bool IsEmpty() const {
        return events.empty();
    }

    void Clear() {
        events.clear();
    }
    
};
