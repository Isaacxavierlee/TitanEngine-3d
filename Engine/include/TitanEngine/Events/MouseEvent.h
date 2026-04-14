#pragma once
#include "Event.h"
#include "TitanEngine/Core/KeyCodes.h"

namespace Titan {

    class MouseMovedEvent : public Event {
    public:
        MouseMovedEvent(float x, float y) : m_X(x), m_Y(y) {}
        float GetX() const { return m_X; }
        float GetY() const { return m_Y; }

        static EventType  GetStaticType() { return EventType::MouseMoved; }
        EventType         GetType()       const override { return GetStaticType(); }
        const char*       GetName()       const override { return "MouseMoved"; }
        EventCategory     GetCategory()   const override {
            return EventCategory::Mouse | EventCategory::Input;
        }
    private:
        float m_X, m_Y;
    };

    class MouseScrolledEvent : public Event {
    public:
        MouseScrolledEvent(float xOff, float yOff) : m_XOffset(xOff), m_YOffset(yOff) {}
        float GetXOffset() const { return m_XOffset; }
        float GetYOffset() const { return m_YOffset; }

        static EventType  GetStaticType() { return EventType::MouseScrolled; }
        EventType         GetType()       const override { return GetStaticType(); }
        const char*       GetName()       const override { return "MouseScrolled"; }
        EventCategory     GetCategory()   const override {
            return EventCategory::Mouse | EventCategory::Input;
        }
    private:
        float m_XOffset, m_YOffset;
    };

    class MouseButtonEvent : public Event {
    public:
        MouseButton   GetButton()     const { return m_Button; }
        EventCategory GetCategory()   const override {
            return EventCategory::Mouse | EventCategory::MouseButton | EventCategory::Input;
        }
    protected:
        explicit MouseButtonEvent(MouseButton b) : m_Button(b) {}
        MouseButton m_Button;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent {
    public:
        explicit MouseButtonPressedEvent(MouseButton b) : MouseButtonEvent(b) {}

        static EventType GetStaticType() { return EventType::MouseButtonPressed; }
        EventType        GetType()  const override { return GetStaticType(); }
        const char*      GetName()  const override { return "MouseButtonPressed"; }
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent {
    public:
        explicit MouseButtonReleasedEvent(MouseButton b) : MouseButtonEvent(b) {}

        static EventType GetStaticType() { return EventType::MouseButtonReleased; }
        EventType        GetType()  const override { return GetStaticType(); }
        const char*      GetName()  const override { return "MouseButtonReleased"; }
    };

} // namespace Titan
