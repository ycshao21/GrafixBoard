#pragma once

#include <functional>

#include "event.hpp"

struct GLFWwindow;

namespace grafix
{

struct WindowConfig
{
    std::string title = "GrafixBoard";
    uint32_t width = 1280;
    uint32_t height = 720;
};

class GRAFIXCORE_API Window
{
public:
    using EventCallbackFn = std::function<void(Event&)>;

    Window(const WindowConfig& config = {});
    ~Window();

    static void pollEvents();

    [[nodiscard]] auto getTitle() const -> const std::string&
    {
        return m_data.title;
    }
    [[nodiscard]] auto getWidth() const -> uint32_t
    {
        return m_data.width;
    }
    [[nodiscard]] auto getHeight() const -> uint32_t
    {
        return m_data.height;
    }
    [[nodiscard]] auto getAspectRatio() const -> float
    {
        return static_cast<float>(m_data.width) /
               static_cast<float>(m_data.height);
    }

    void setEventCallback(const EventCallbackFn& callback)
    {
        m_data.eventCallback = callback;
    }

    [[nodiscard]] auto getHandle() const -> GLFWwindow*
    {
        return m_handle;
    }

private:
    GLFWwindow* m_handle = nullptr;

    struct WindowData
    {
        std::string title;
        uint32_t width;
        uint32_t height;

        EventCallbackFn eventCallback;
    } m_data{};
};
}  // namespace grafix
