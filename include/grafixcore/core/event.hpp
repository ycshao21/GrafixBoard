#pragma once

// Adapted from The Cherno's Hazel Engine

#include <format>
#include <string>

#include "codes.hpp"

#define BIT(x) (1 << x)

namespace grafix
{

enum class EventType : uint8_t
{
    None = 0,
    WindowClose,
    WindowResize,
    WindowFocus,
    WindowLostFocus,
    WindowMoved,
    AppTick,
    AppUpdate,
    AppRender,
    KeyPressed,
    KeyReleased,
    KeyTyped,
    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved,
    MouseScrolled
};

enum EventCategory : uint8_t
{
    EventCategoryNone = 0,
    EventCategoryApplication = BIT(0),
    EventCategoryInput = BIT(1),
    EventCategoryKeyboard = BIT(2),
    EventCategoryMouse = BIT(3),
    EventCategoryMouseButton = BIT(4)
};

#define EVENT_CLASS_TYPE(type)                                                 \
    static auto getStaticType() -> EventType                                   \
    {                                                                          \
        return EventType::type;                                                \
    }                                                                          \
    auto getEventType() const -> EventType override                            \
    {                                                                          \
        return getStaticType();                                                \
    }                                                                          \
    auto getName() const -> const char* override                               \
    {                                                                          \
        return #type;                                                          \
    }

#define EVENT_CLASS_CATEGORY(category)                                         \
    auto getCategoryFlags() const -> uint8_t override                          \
    {                                                                          \
        return category;                                                       \
    }

class Event
{
public:
    [[nodiscard]] virtual auto getEventType() const -> EventType = 0;
    [[nodiscard]] virtual auto getName() const -> const char* = 0;
    [[nodiscard]] virtual auto getCategoryFlags() const -> uint8_t = 0;
    [[nodiscard]] virtual auto toString() const -> std::string
    {
        return getName();
    }

    [[nodiscard]] auto isInCategory(EventCategory category) const -> bool
    {
        return static_cast<bool>(getCategoryFlags() & category);
    }

    bool handled = false;
};

class EventDispatcher
{
public:
    EventDispatcher(Event& event) : m_event(event)
    {
    }

    template <typename T, typename F>
    auto dispatch(const F& func) -> bool
    {
        if (m_event.getEventType() == T::getStaticType()) {
            // VT_CORE_TRACE("Event dispatched: {0}", event_.to_string());
            m_event.handled |= func(static_cast<T&>(m_event));
            return true;
        }
        return false;
    }

private:
    Event& m_event;
};

// Application Events

class WindowResizeEvent : public Event
{
public:
    WindowResizeEvent(uint32_t width, uint32_t height)
        : m_width(width), m_height(height)
    {
    }

    [[nodiscard]] auto getWidth() const -> uint32_t
    {
        return m_width;
    }
    [[nodiscard]] auto getHeight() const -> uint32_t
    {
        return m_height;
    }

    [[nodiscard]] auto toString() const -> std::string override
    {
        return std::format("WindowResizeEvent: ({0}, {1})", m_width, m_height);
    }

    EVENT_CLASS_TYPE(WindowResize)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)

private:
    uint32_t m_width, m_height;
};

class WindowCloseEvent : public Event
{
public:
    EVENT_CLASS_TYPE(WindowClose)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppTickEvent : public Event
{
public:
    EVENT_CLASS_TYPE(AppTick)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppUpdateEvent : public Event
{
public:
    EVENT_CLASS_TYPE(AppUpdate)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppRenderEvent : public Event
{
public:
    EVENT_CLASS_TYPE(AppRender)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

// Key Events

class KeyEvent : public Event
{
public:
    [[nodiscard]] auto getKey() const -> Key
    {
        return m_key;
    }

    EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
protected:
    KeyEvent(Key key) : m_key(key)
    {
    }

    Key m_key;
};

class KeyPressedEvent : public KeyEvent
{
public:
    KeyPressedEvent(Key key, bool isRepeat)
        : KeyEvent(key), m_isRepeat(isRepeat)
    {
    }

    [[nodiscard]] auto isRepeat() const -> bool
    {
        return m_isRepeat;
    }

    [[nodiscard]] auto toString() const -> std::string override
    {
        return std::format("KeyPressedEvent: {0} (repeat = {1})",
                           (uint16_t) m_key, (uint16_t) m_isRepeat);
    }

    EVENT_CLASS_TYPE(KeyPressed)
private:
    bool m_isRepeat;
};

class KeyReleasedEvent : public KeyEvent
{
public:
    KeyReleasedEvent(Key key) : KeyEvent(key)
    {
    }

    [[nodiscard]] auto toString() const -> std::string override
    {
        return std::format("KeyReleasedEvent: {0}", (uint16_t) m_key);
    }

    EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent final : public KeyEvent
{
public:
    KeyTypedEvent(Key key) : KeyEvent(key)
    {
    }

    [[nodiscard]] auto toString() const -> std::string override
    {
        return std::format("KeyTypedEvent: {0}", (uint16_t) m_key);
    }

    EVENT_CLASS_TYPE(KeyTyped)
};

// Mouse Events

class MouseMovedEvent : public Event
{
public:
    MouseMovedEvent(float x, float y) : m_mouseX(x), m_mouseY(y)
    {
    }

    [[nodiscard]] auto getX() const -> float
    {
        return m_mouseX;
    }
    [[nodiscard]] auto getY() const -> float
    {
        return m_mouseY;
    }

    [[nodiscard]] auto toString() const -> std::string override
    {
        return std::format("MouseMovedEvent: ({0}, {1})", m_mouseX, m_mouseY);
    }

    EVENT_CLASS_TYPE(MouseMoved)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
protected:
    float m_mouseX, m_mouseY;
};

class MouseScrolledEvent : public Event
{
public:
    MouseScrolledEvent(float x_offset, float y_offset)
        : m_xOffset(x_offset), m_yOffset(y_offset)
    {
    }

    [[nodiscard]] auto getXOffset() const -> float
    {
        return m_xOffset;
    }
    [[nodiscard]] auto getYOffset() const -> float
    {
        return m_yOffset;
    }

    [[nodiscard]] auto toString() const -> std::string override
    {
        return std::format("MouseScrolledEvent: ({0}, {1})", m_xOffset,
                           m_yOffset);
    }

    EVENT_CLASS_TYPE(MouseScrolled)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
protected:
    float m_xOffset, m_yOffset;
};

class MouseButtonEvent : public Event
{
public:
    [[nodiscard]] MouseButton getMouseButton() const
    {
        return m_button;
    }

    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
protected:
    MouseButtonEvent(MouseButton button) : m_button(button)
    {
    }
    MouseButton m_button;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
    MouseButtonPressedEvent(MouseButton button) : MouseButtonEvent(button)
    {
    }

    [[nodiscard]] auto toString() const -> std::string override
    {
        return std::format("MouseButtonPressedEvent: {0}", (uint16_t) m_button);
    }

    EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
    MouseButtonReleasedEvent(MouseButton button) : MouseButtonEvent(button)
    {
    }

    [[nodiscard]] auto toString() const -> std::string override
    {
        return std::format("MouseButtonReleasedEvent: {0}",
                           (uint16_t) m_button);
    }

    EVENT_CLASS_TYPE(MouseButtonReleased)
};

}  // namespace grafix