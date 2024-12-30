//
// Created by Administrator on 12/29/2024.
//

#ifndef EVENT_H
#define EVENT_H

#define BIT(x) (1u << x)

#include <cstdint>
#include <string>

#include <glm/glm.hpp>

#define EVENT_CLASS_TYPE(type) \
static EventType GetStaticType() { return EventType::type; } \
virtual EventType GetEventType() const override { return GetStaticType(); } \
virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
virtual int32_t GetEventCategory() const override { return category; }

namespace Core::Engine {
    enum class EventType : uint32_t {
        None = 0, WindowClose, WindowMinimize, WindowMaximize, WindowResize, WindowMove, WindowFocus,
        KeyPress, KeyHold, KeyRelease, KeyType, MouseButtonPress, MouseButtonRelease, MouseMove, MouseScroll
    };

    enum EventCategory : uint32_t {
        None = 0, EventCategoryWindow = BIT(0), EventCategoryInput = BIT(1), EventCategoryKeyboard = BIT(2), EventCategoryMouse = BIT(3)
    };

    class Event {
    public:
        virtual ~Event() {}

        virtual const char* GetName() const = 0;
        virtual EventType GetEventType() const = 0;
        virtual int32_t GetEventCategory() const = 0;

        virtual std::string ToString() const { return GetName(); }
        inline bool IsInCategory(EventCategory category) { return GetEventCategory() & category; }
    };
}

#endif //EVENT_H
