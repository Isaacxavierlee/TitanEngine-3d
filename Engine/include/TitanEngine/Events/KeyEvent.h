#pragma once
#include "Event.h"
#include "TitanEngine/Core/KeyCodes.h"

namespace Titan {

    class KeyEvent : public Event {
    public:
        Key           GetKey()      const { return m_Key; }
        EventCategory GetCategory() const override {
            return EventCategory::Keyboard | EventCategory::Input;
        }
    protected:
        explicit KeyEvent(Key k) : m_Key(k) {}
        Key m_Key;
    };

    class KeyPressedEvent : public KeyEvent {
    public:
        KeyPressedEvent(Key k, bool repeat) : KeyEvent(k), m_Repeat(repeat) {}
        bool IsRepeat() const { return m_Repeat; }

        static EventType GetStaticType() { return EventType::KeyPressed; }
        EventType        GetType()  const override { return GetStaticType(); }
        const char*      GetName()  const override { return "KeyPressed"; }
    private:
        bool m_Repeat;
    };

    class KeyReleasedEvent : public KeyEvent {
    public:
        explicit KeyReleasedEvent(Key k) : KeyEvent(k) {}

        static EventType GetStaticType() { return EventType::KeyReleased; }
        EventType        GetType()  const override { return GetStaticType(); }
        const char*      GetName()  const override { return "KeyReleased"; }
    };

} // namespace Titan
