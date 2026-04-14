#pragma once
#include "Event.h"

namespace Titan {

    class WindowCloseEvent : public Event {
    public:
        static EventType  GetStaticType() { return EventType::WindowClose; }
        EventType         GetType()       const override { return GetStaticType(); }
        const char*       GetName()       const override { return "WindowClose"; }
        EventCategory     GetCategory()   const override { return EventCategory::Application; }
    };

    class WindowResizeEvent : public Event {
    public:
        WindowResizeEvent(int w, int h) : m_Width(w), m_Height(h) {}

        int GetWidth()  const { return m_Width; }
        int GetHeight() const { return m_Height; }

        static EventType  GetStaticType() { return EventType::WindowResize; }
        EventType         GetType()       const override { return GetStaticType(); }
        const char*       GetName()       const override { return "WindowResize"; }
        EventCategory     GetCategory()   const override { return EventCategory::Application; }

    private:
        int m_Width, m_Height;
    };

} // namespace Titan
