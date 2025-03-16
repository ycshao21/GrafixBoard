#pragma once

// #include <vulkan/vulkan.h>

// #include "Window.h"

// #include "Grafix/Events/ApplicationEvent.h"
// #include "Grafix/ImGui/ImGuiLayer.h"

// #include "Grafix/Utils/Stopwatch.h"

#include <functional>
#include <memory>
#include <string>

namespace grafix
{
struct ApplicationConfig
{
    std::string name = "GrafixBoard";
    uint32_t width = 1280;
    uint32_t height = 720;
    // RendererAPI renderer_api = RendererAPI::Vulkan;
    float target_frame_rate = 60.0F;
};

class Application
{
public:
    Application(const ApplicationConfig& config);
    virtual ~Application();

    void run();

    void close()
    {
        m_running = false;
    }

    static auto get() -> Application&
    {
        return *s_instance;
    }

    // void OnEvent(Event& e);

    // Window
    // inline Window& GetWindow() { return *m_Window; }
    // inline const Window& GetWindow() const { return *m_Window; }

    //     void PushLayer(Layer* layer);

    //     inline bool IsMinimized() { return m_Minimized; }
    //     inline ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

    //     // Vulkan
    //     static VkInstance GetInstance();
    //     static VkPhysicalDevice GetPhysicalDevice();
    //     static VkDevice GetDevice();

    //     static VkCommandBuffer GetCommandBuffer();
    //     static void FlushCommandBuffer(VkCommandBuffer commandBuffer);
    //     static void SubmitResourceFree(std::function<void()>&& func);
    // private:
    //     bool OnWindowClose(WindowCloseEvent& e);
    //     bool OnWindowResize(WindowResizeEvent& e);
    //
    //     // Vulkan
    //     void InitVulkan();
    //     void CleanupVulkan();

    //     void CreateVkInstance();
    //     void SelectPhysicalDevice();
    //     void SelectGraphicsQueueFamily();
    //     void CreateLogicalDevice();
    //     void CreateWindowSurface();
    //     void CreateDescriptorPool();
    //     void CreateFramebuffers();
private:
    inline static Application* s_instance = nullptr;

    bool m_running = true;
    bool m_minimized = false;

    //     std::unique_ptr<Window> m_Window;

    //     std::vector<Layer*> m_LayerStack;
    //     ImGuiLayer* m_ImGuiLayer;

    float m_last_frame_time = 0.0F;
    float m_delta_time = 0.0F;
};

// Define the function in your own application.
auto create_application() -> Application*;

}  // namespace grafix
