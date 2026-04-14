#pragma once
#include <functional>
#include <string>

namespace Titan {

    enum class EventType {
        None = 0,
        WindowClose, WindowResize,
        KeyPressed, KeyReleased,
        MouseButtonPressed, MouseButtonReleased,
        MouseMoved, MouseScrolled
    };

    enum class EventCategory : int {
        None        = 0,
        Application = 1 << 0,
        Input       = 1 << 1,
        Keyboard    = 1 << 2,
        Mouse       = 1 << 3,
        MouseButton = 1 << 4,
    };

    inline int operator&(EventCategory a, EventCategory b) {
        return static_cast<int>(a) & static_cast<int>(b);
    }
    inline EventCategory operator|(EventCategory a, EventCategory b) {
        return static_cast<EventCategory>(static_cast<int>(a) | static_cast<int>(b));
    }

    class Event {
    public:
        virtual ~Event() = default;
        virtual EventType     GetType()     const = 0;
        virtual const char*   GetName()     const = 0;
        virtual EventCategory GetCategory() const = 0;

        bool IsInCategory(EventCategory c) const { return (GetCategory() & c) != 0; }
        bool Handled = false;
    };

    class EventDispatcher {
    public:
        explicit EventDispatcher(Event& e) : m_Event(e) {}

        template<typename T>
        bool Dispatch(std::function<bool(T&)> fn) {
            if (m_Event.GetType() == T::GetStaticType()) {
                m_Event.Handled |= fn(static_cast<T&>(m_Event));
                return true;
            }
            return false;
        }
    private:
        Event& m_Event;
    };

} // namespace Titan
