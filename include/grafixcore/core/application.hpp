#pragma once

#include <functional>
#include <memory>
#include <string>

#include <imgui_impl_vulkan.h>
#include <vulkan/vulkan.h>

#include "grafixcore/ui/ui_layer.hpp"
#include "platform.hpp"
#include "window.hpp"

namespace grafix
{
struct ApplicationConfig
{
    std::string name = "GrafixBoard";
    uint32_t width = 1280;
    uint32_t height = 720;
    // RendererAPI rendererAPI = RendererAPI::Vulkan;
    float targetFrameRate = 60.0f;
};

class Application
{
public:
    Application(const ApplicationConfig& config);
    virtual ~Application();

    void run();

    [[nodiscard]] auto isMinimized() const -> bool
    {
        return m_state.isMinimized;
    }

    static auto get() -> Application&;

    auto getWindow() -> Window&
    {
        return *m_window;
    }
    auto getImGuiLayer() -> ImGuiLayer*
    {
        return m_imguiLayer;
    }

    void pushLayer(Layer* layer);
    void onEvent(Event& e);

    //     // Vulkan
    //     static VkInstance GetInstance();
    //     static VkPhysicalDevice GetPhysicalDevice();
    //     static VkDevice GetDevice();

    //     static VkCommandBuffer GetCommandBuffer();
    //     static void FlushCommandBuffer(VkCommandBuffer commandBuffer);
    //     static void SubmitResourceFree(std::function<void()>&& func);
private:
    auto onWindowClose(WindowCloseEvent& e) -> bool;
    auto onWindowResize(WindowResizeEvent& e) -> bool;

    // Vulkan
    void setupVulkan();
    void setupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface,
                           int width, int height);
    void cleanupVulkan();

    static void createVkInstance();
    static void createLogicalDevice();
    static void createDescriptorPool();

    struct ApplicationState
    {
        bool isRunning = true;
        bool isMinimized = false;
        bool isFrameRateLimited = false;
    } m_state;

    float m_lastTime = 0.0f;
    float m_deltaTime = 0.0f;
    float m_targetFrameTime = 1.0f / 60.0f;

    std::unique_ptr<Window> m_window = nullptr;

    std::vector<Layer*> m_layerStack;
    ImGuiLayer* m_imguiLayer;
};

// Define the function in your own application.
auto createApplication() -> Application*;

}  // namespace grafix
