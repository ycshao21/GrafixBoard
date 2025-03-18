#include <GLFW/glfw3.h>

#include "grafixcore/core/application.hpp"
#include "grafixcore/core/assert.hpp"
#include "grafixcore/core/window.hpp"
#include "grafixcore/utils/log.hpp"

namespace grafix
{
static bool s_glfwInitialized = false;

Window::Window(const WindowConfig& config)
{
    m_data.title = config.title;
    m_data.width = config.width;
    m_data.height = config.height;

    // Initialize GLFW
    if (!s_glfwInitialized) {
        [[maybe_unused]] int success = glfwInit();
        GB_ASSERT(success, "Window: GLFW initialization failed!");
        GB_CORE_TRACE("Window: GLFW initialized.");
        s_glfwInitialized = true;
    }

    // TODO: Vulkan does not require an OpenGL context, but OpenGL does.
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    m_handle = glfwCreateWindow(m_data.width, m_data.height,
                                m_data.title.c_str(), nullptr, nullptr);
    GB_ASSERT(m_handle, "Window: Creation failed!");
    GB_CORE_INFO("Window: Showing: {0} ({1}, {2})", m_data.title, m_data.width,
                 m_data.height);

    glfwSetWindowUserPointer(m_handle, &m_data);

    // Set callbacks
    glfwSetErrorCallback([](int error, const char* description) {
        GB_CORE_ERROR("Window: GLFW Error ({0}): {1}", error, description);
    });

    glfwSetWindowSizeCallback(
        m_handle, [](GLFWwindow* window, int width, int height) {
            WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);
            data.width = width;
            data.height = height;

            WindowResizeEvent e(width, height);
            data.eventCallback(e);
        });

    glfwSetWindowCloseCallback(m_handle, [](GLFWwindow* window) {
        WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);

        WindowCloseEvent e;
        data.eventCallback(e);
    });

    glfwSetKeyCallback(m_handle, [](GLFWwindow* window, int keycode,
                                    [[maybe_unused]] int scancode, int action,
                                    [[maybe_unused]] int mods) {
        WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);
        Key key = (Key) keycode;

        switch (action) {
        case GLFW_PRESS: {
            KeyPressedEvent e(key, 0);
            data.eventCallback(e);
            break;
        }
        case GLFW_RELEASE: {
            KeyReleasedEvent e(key);
            data.eventCallback(e);
            break;
        }
        case GLFW_REPEAT: {
            KeyPressedEvent e(key, true);
            data.eventCallback(e);
            break;
        }
        }
    });

    glfwSetCharCallback(m_handle, [](GLFWwindow* window, unsigned int keycode) {
        WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);

        KeyTypedEvent e((Key) keycode);
        data.eventCallback(e);
    });

    glfwSetMouseButtonCallback(m_handle, [](GLFWwindow* window, int buttonCode,
                                            int action,
                                            [[maybe_unused]] int mods) {
        WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);
        auto button = (MouseButton) buttonCode;

        switch (action) {
        case GLFW_PRESS: {
            MouseButtonPressedEvent e(button);
            data.eventCallback(e);
            break;
        }
        case GLFW_RELEASE: {
            MouseButtonReleasedEvent e(button);
            data.eventCallback(e);
            break;
        }
        }
    });

    glfwSetScrollCallback(
        m_handle, [](GLFWwindow* window, double xOffset, double yOffset) {
            WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);

            MouseScrolledEvent e((float) xOffset, (float) yOffset);
            data.eventCallback(e);
        });

    glfwSetCursorPosCallback(
        m_handle, [](GLFWwindow* window, double xPos, double yPos) {
            WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);

            MouseMovedEvent e((float) xPos, (float) yPos);
            data.eventCallback(e);
        });
}

Window::~Window()
{
    glfwDestroyWindow(m_handle);
    m_handle = nullptr;
    GB_CORE_INFO("Window: Destroyed...");

    glfwTerminate();
    s_glfwInitialized = false;
    GB_CORE_TRACE("Window: GLFW terminated.");
}

void Window::pollEvents()
{
    glfwPollEvents();
}

}  // namespace grafix