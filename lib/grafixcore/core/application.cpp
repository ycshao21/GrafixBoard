#include <ranges>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>

#include "grafixcore/core/application.hpp"
#include "grafixcore/core/assert.hpp"
#include "grafixcore/core/platform.hpp"
#include "grafixcore/utils/log.hpp"

// This code is adapted from ImGui/examples/example_glfw_vulkan/main.cpp and
// Walnut by The Cherno.

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to
// maximize ease of testing and compatibility with old VS compilers. To link
// with VS2010-era libraries, VS2015+ requires linking with
// legacy_stdio_definitions.lib, which we do using this pragma. Your own project
// should not be affected, as you are likely to link with a newer binary of GLFW
// that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) &&                                 \
    !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
    #pragma comment(lib, "legacy_stdio_definitions")
#endif

#define APP_USE_UNLIMITED_FRAME_RATE

#ifdef GB_DEBUG_BUILD
    #define APP_USE_VULKAN_DEBUG_REPORT
#endif

// Vulkan Data
static VkAllocationCallbacks* g_Allocator = nullptr;
static VkInstance g_Instance = VK_NULL_HANDLE;
static VkPhysicalDevice g_PhysicalDevice = VK_NULL_HANDLE;
static VkDevice g_Device = VK_NULL_HANDLE;
static uint32_t g_QueueFamily = static_cast<uint32_t>(-1);
static VkQueue g_Queue = VK_NULL_HANDLE;
[[maybe_unused]] static VkDebugReportCallbackEXT g_DebugReport = VK_NULL_HANDLE;
static VkPipelineCache g_PipelineCache = VK_NULL_HANDLE;
static VkDescriptorPool g_DescriptorPool = VK_NULL_HANDLE;

static ImGui_ImplVulkanH_Window g_MainWindowData{};
static uint32_t g_MinImageCount = 2;
static bool g_SwapChainRebuild = false;

// // Per-frame-in-flight
// static std::vector<std::vector<VkCommandBuffer>> s_CommandBuffers;
// static std::vector<std::vector<std::function<void()>>> s_ResourceFreeQueue;

// Unlike g_MainWindowData.FrameIndex, this is not the the swapchain image index
// and is always guaranteed to increase (eg. 0, 1, 2, 0, 1, 2)
// static uint32_t s_CurrentFrameIndex = 0;

// TODO: Remove
static GLFWwindow* s_WindowHandle = nullptr;

#ifdef APP_USE_VULKAN_DEBUG_REPORT
static VKAPI_ATTR auto VKAPI_CALL vkDebugCallback(
    VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType,
    uint64_t object, size_t location, int32_t messageCode,
    const char* pLayerPrefix, const char* pMessage, void* pUserData) -> VkBool32
{
    (void) flags;
    (void) object;
    (void) location;
    (void) messageCode;
    (void) pUserData;
    (void) pLayerPrefix;  // Unused arguments

    GB_CORE_TRACE("Vulkan debug report from ObjectType: {0}, message: {1}",
                  (int) objectType, pMessage);
    return VK_FALSE;
}
#endif  // APP_USE_VULKAN_DEBUG_REPORT

void checkVkResult(VkResult result)
{
    GB_ASSERT(result == VK_SUCCESS, "Vulkan error: {}!",
              string_VkResult(result));
}

static auto isExtensionAvailable(
    const std::vector<VkExtensionProperties>& properties, const char* extension)
    -> bool
{
    for (const VkExtensionProperties& p : properties) {
        if (strcmp(p.extensionName, extension) == 0) {
            return true;
        }
    }
    return false;
}

void frameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* drawData)
{
    VkSemaphore imageAcquiredSemaphore =
        wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
    VkSemaphore renderCompleteSemaphore =
        wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
    VkResult err = vkAcquireNextImageKHR(g_Device, wd->Swapchain, UINT64_MAX,
                                         imageAcquiredSemaphore, VK_NULL_HANDLE,
                                         &wd->FrameIndex);
    if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR) {
        g_SwapChainRebuild = true;
    }
    if (err == VK_ERROR_OUT_OF_DATE_KHR) {
        return;
    }
    if (err != VK_SUBOPTIMAL_KHR) {
        checkVkResult(err);
    }

    // s_CurrentFrameIndex = (s_CurrentFrameIndex + 1) % wd->ImageCount;

    ImGui_ImplVulkanH_Frame* fd = &wd->Frames[wd->FrameIndex];
    {
        err = vkWaitForFences(
            g_Device, 1, &fd->Fence, VK_TRUE,
            UINT64_MAX);  // wait indefinitely instead of periodically checking
        checkVkResult(err);

        err = vkResetFences(g_Device, 1, &fd->Fence);
        checkVkResult(err);
    }

    // // Free resources in queue
    // {
    //     for (auto& func : s_ResourceFreeQueue[s_CurrentFrameIndex]) {
    //         func();
    //     }
    //     s_ResourceFreeQueue[s_CurrentFrameIndex].clear();
    // }

    {
        // auto& allocatedCommandBuffers = s_CommandBuffers[wd->FrameIndex];
        // if (allocatedCommandBuffers.size() > 0) {
        //     vkFreeCommandBuffers(g_Device, fd->CommandPool,
        //                          (uint32_t) allocatedCommandBuffers.size(),
        //                          allocatedCommandBuffers.data());
        //     allocatedCommandBuffers.clear();
        // }

        err = vkResetCommandPool(g_Device, fd->CommandPool, 0);
        checkVkResult(err);

        // Begin command buffer
        VkCommandBufferBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
        checkVkResult(err);
    }

    {
        VkRenderPassBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        info.renderPass = wd->RenderPass;
        info.framebuffer = fd->Framebuffer;
        info.renderArea.extent.width = wd->Width;
        info.renderArea.extent.height = wd->Height;
        info.clearValueCount = 1;
        info.pClearValues = &wd->ClearValue;
        vkCmdBeginRenderPass(fd->CommandBuffer, &info,
                             VK_SUBPASS_CONTENTS_INLINE);
    }

    ImGui_ImplVulkan_RenderDrawData(drawData, fd->CommandBuffer);

    // Submit command buffer
    vkCmdEndRenderPass(fd->CommandBuffer);
    {
        VkPipelineStageFlags waitStage =
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkSubmitInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = &imageAcquiredSemaphore;
        info.pWaitDstStageMask = &waitStage;
        info.commandBufferCount = 1;
        info.pCommandBuffers = &fd->CommandBuffer;
        info.signalSemaphoreCount = 1;
        info.pSignalSemaphores = &renderCompleteSemaphore;

        err = vkEndCommandBuffer(fd->CommandBuffer);
        checkVkResult(err);
        err = vkQueueSubmit(g_Queue, 1, &info, fd->Fence);
        checkVkResult(err);
    }
}

void framePresent()
{
    if (g_SwapChainRebuild) {
        return;
    }

    ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;

    VkSemaphore renderCompleteSemaphore =
        wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
    VkPresentInfoKHR info = {};
    info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores = &renderCompleteSemaphore;
    info.swapchainCount = 1;
    info.pSwapchains = &wd->Swapchain;
    info.pImageIndices = &wd->FrameIndex;

    VkResult err = vkQueuePresentKHR(g_Queue, &info);
    if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR) {
        g_SwapChainRebuild = true;
    }
    if (err == VK_ERROR_OUT_OF_DATE_KHR) {
        return;
    }
    if (err != VK_SUBOPTIMAL_KHR) {
        checkVkResult(err);
    }

    wd->SemaphoreIndex =
        (wd->SemaphoreIndex + 1) %
        wd->SemaphoreCount;  // Now we can use the next set of semaphores
}

static void setupRendererBackends()
{
    ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForVulkan(s_WindowHandle, true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = g_Instance;
    init_info.PhysicalDevice = g_PhysicalDevice;
    init_info.Device = g_Device;
    init_info.QueueFamily = g_QueueFamily;
    init_info.Queue = g_Queue;
    init_info.PipelineCache = g_PipelineCache;
    init_info.DescriptorPool = g_DescriptorPool;
    init_info.RenderPass = wd->RenderPass;
    init_info.Subpass = 0;
    init_info.MinImageCount = g_MinImageCount;
    init_info.ImageCount = wd->ImageCount;
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.Allocator = g_Allocator;
    init_info.CheckVkResultFn = checkVkResult;
    ImGui_ImplVulkan_Init(&init_info);
}

// static void loadFonts()
// {
//     // Load font
//     ImFontConfig fontConfig;
//     fontConfig.FontDataOwnedByAtlas = false;

//     ImGuiIO& io = ImGui::GetIO();
//     ImFont* font =
//         io.Fonts->AddFontFromFileTTF("../Grafix/assets/arialbd.ttf", 120.0f);
//     io.FontDefault = font;

//     VkResult err;
//     ImGui_ImplVulkanH_Window* wd = &g_WindowData;
//     // Upload Fonts
//     {
//         // Use any command queue
//         VkCommandPool commandPool = wd->Frames[wd->FrameIndex].CommandPool;
//         VkCommandBuffer commandBuffer =
//             wd->Frames[wd->FrameIndex].CommandBuffer;

//         err = vkResetCommandPool(g_Device, commandPool, 0);
//         GB_ASSERT(err == VK_SUCCESS, "Cannot reset command pool!");

//         VkCommandBufferBeginInfo beginInfo = {};
//         beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//         beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

//         err = vkBeginCommandBuffer(commandBuffer, &beginInfo);
//         GB_ASSERT(err == VK_SUCCESS, "Cannot begin command buffer!");

//         ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);

//         VkSubmitInfo endInfo = {};
//         endInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
//         endInfo.commandBufferCount = 1;
//         endInfo.pCommandBuffers = &commandBuffer;

//         err = vkEndCommandBuffer(commandBuffer);
//         GB_ASSERT(err == VK_SUCCESS, "Cannot end command buffer!");

//         err = vkQueueSubmit(g_Queue, 1, &endInfo, VK_NULL_HANDLE);
//         GB_ASSERT(err == VK_SUCCESS, "Cannot submit queue!");

//         err = vkDeviceWaitIdle(g_Device);
//         GB_ASSERT(err == VK_SUCCESS, "Device is still busy!");

//         ImGui_ImplVulkan_DestroyFontUploadObjects();
//     }
// }

namespace grafix
{
static Application* s_instance = nullptr;

Application::Application(const ApplicationConfig& config)
{
    GB_ASSERT(!s_instance, "Application: Instance already exists!");
    s_instance = this;

    GB_CORE_INFO("Application: Launching {}", config.name);
    m_targetFrameTime = 1.0f / config.targetFrameRate;

    WindowConfig windowConfig{};
    windowConfig.title = config.name;
    windowConfig.width = config.width;
    windowConfig.height = config.height;
    m_window = std::make_unique<Window>(windowConfig);
    m_window->setEventCallback([this](Event& e) { onEvent(e); });
    // TODO: Remove this
    s_WindowHandle = m_window->getHandle();

    setupVulkan();

    // Create Window Surface
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkResult err = glfwCreateWindowSurface(g_Instance, m_window->getHandle(),
                                           g_Allocator, &surface);
    checkVkResult(err);

    // Setup Framebuffers
    int width, height;
    glfwGetFramebufferSize(m_window->getHandle(), &width, &height);
    ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;
    setupVulkanWindow(wd, surface, width, height);

    m_imguiLayer = new ImGuiLayer();
    pushLayer(m_imguiLayer);
    setupRendererBackends();
    // loadFonts();

    // s_CommandBuffers.resize(wd->ImageCount);
    // s_ResourceFreeQueue.resize(wd->ImageCount);

    GB_CORE_INFO("Application: {} launched successfully.", config.name);
}

Application::~Application()
{
    GB_CORE_INFO("Application: Shutting down...");

    VkResult err = vkDeviceWaitIdle(g_Device);
    checkVkResult(err);

    for (Layer* layer : m_layerStack) {
        layer->onDetach();
        delete layer;
    }
    m_layerStack.clear();

    ImGui_ImplVulkanH_DestroyWindow(g_Instance, g_Device, &g_MainWindowData,
                                    g_Allocator);
    cleanupVulkan();

    m_window.reset();

    s_instance = nullptr;
    GB_CORE_INFO("Application: Shut down successfully. End.");
}

auto Application::get() -> Application&
{
    GB_ASSERT(s_instance, "Application: Instance does not exist!");
    return *s_instance;
}

void Application::pushLayer(Layer* layer)
{
    m_layerStack.push_back(layer);
    layer->onAttach();
}

void Application::onEvent(Event& e)
{
    EventDispatcher dispatcher(e);

    dispatcher.dispatch<WindowCloseEvent>(
        [this](WindowCloseEvent& e) { return onWindowClose(e); });
    dispatcher.dispatch<WindowResizeEvent>(
        [this](WindowResizeEvent& e) { return onWindowResize(e); });

    for (auto& layer : std::ranges::reverse_view(m_layerStack)) {
        if (e.handled) {
            break;
        }
        layer->onEvent(e);
    }
}

void Application::run()
{
    [[maybe_unused]] float runningTime = 0.0f;
    m_lastTime = platform::getTime();

    // Main loop
    while (m_state.isRunning) {
        m_window->pollEvents();

        // Update all layers if the window is not minimized
        if (!m_state.isMinimized) {
            for (Layer* layer : m_layerStack) {
                layer->onUpdate(m_deltaTime);
            }
        }

        if (g_SwapChainRebuild) {
            int fb_width, fb_height;
            glfwGetFramebufferSize(m_window->getHandle(), &fb_width,
                                   &fb_height);
            if (fb_width > 0 && fb_height > 0) {
                ImGui_ImplVulkan_SetMinImageCount(g_MinImageCount);
                ImGui_ImplVulkanH_CreateOrResizeWindow(
                    g_Instance, g_PhysicalDevice, g_Device, &g_MainWindowData,
                    g_QueueFamily, g_Allocator, fb_width, fb_height,
                    g_MinImageCount);
                g_MainWindowData.FrameIndex = 0;
                g_SwapChainRebuild = false;
                // // Clear allocated command buffers from here since entire
                // // pool is destroyed
                // s_CommandBuffers.clear();
                // s_CommandBuffers.resize(wd->ImageCount);
            }
        }

        if (glfwGetWindowAttrib(m_window->getHandle(), GLFW_ICONIFIED) != 0) {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        ImGuiLayer::beginFrame();
        for (Layer* layer : m_layerStack) {
            layer->onUIRender();
        }
        ImGuiLayer::endFrame(&g_MainWindowData);

        float currentTime = platform::getTime();
        float elapsedTime = currentTime - m_lastTime;

        // Limit frame rate
        if (m_state.isFrameRateLimited) {
            if (elapsedTime < m_targetFrameTime) {
                platform::sleep(m_targetFrameTime - elapsedTime);
                elapsedTime = m_targetFrameTime;
            }
        }

        m_lastTime = currentTime;
        m_deltaTime = elapsedTime;
        runningTime += elapsedTime;
    }
}

auto Application::onWindowClose([[maybe_unused]] WindowCloseEvent& e) -> bool
{
    m_state.isRunning = false;
    return true;
}

auto Application::onWindowResize(WindowResizeEvent& e) -> bool
{
    if (e.getWidth() == 0 || e.getHeight() == 0) {
        m_state.isMinimized = true;
        return false;
    }

    m_state.isMinimized = false;
    return true;
}

void Application::setupVulkan()
{
#ifdef IMGUI_IMPL_VULKAN_USE_VOLK
    volkInitialize();
#endif

    GB_ASSERT(glfwVulkanSupported(),
              "Vulkan: GLFW failed to find Vulkan support!");

    createVkInstance();

    // Select Physical Device (GPU)
    g_PhysicalDevice = ImGui_ImplVulkanH_SelectPhysicalDevice(g_Instance);
    GB_ASSERT(g_PhysicalDevice, "Vulkan: No physical device found!");

    // Select graphics queue family
    g_QueueFamily = ImGui_ImplVulkanH_SelectQueueFamilyIndex(g_PhysicalDevice);
    GB_ASSERT(g_QueueFamily != (uint32_t) -1,
              "Vulkan: No queue families found!");

    createLogicalDevice();
    createDescriptorPool();
}

void Application::createVkInstance()
{
    VkResult err;

    uint32_t extensions_count = 0;
    const char** glfw_extensions =
        glfwGetRequiredInstanceExtensions(&extensions_count);

    std::vector<const char*> instance_extensions;
    instance_extensions.reserve(extensions_count);
    for (uint32_t i = 0; i < extensions_count; i++) {
        instance_extensions.push_back(glfw_extensions[i]);
    }

    VkInstanceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

    // Enumerate available extensions
    uint32_t properties_count;
    std::vector<VkExtensionProperties> properties;
    vkEnumerateInstanceExtensionProperties(nullptr, &properties_count, nullptr);
    properties.resize(properties_count);
    err = vkEnumerateInstanceExtensionProperties(nullptr, &properties_count,
                                                 properties.data());
    checkVkResult(err);

    // Enable required extensions
    if (isExtensionAvailable(
            properties,
            VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME)) {
        instance_extensions.push_back(
            VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    }
#ifdef VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME
    if (isExtensionAvailable(properties,
                             VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME)) {
        instance_extensions.push_back(
            VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    }
#endif

    // Enabling validation layers
#ifdef APP_USE_VULKAN_DEBUG_REPORT
    const char* layers[] = {"VK_LAYER_KHRONOS_validation"};
    create_info.enabledLayerCount = 1;
    create_info.ppEnabledLayerNames = layers;
    instance_extensions.push_back("VK_EXT_debug_report");
#endif

    // Create Vulkan Instance
    create_info.enabledExtensionCount = (uint32_t) instance_extensions.size();
    create_info.ppEnabledExtensionNames = instance_extensions.data();
    err = vkCreateInstance(&create_info, g_Allocator, &g_Instance);
    checkVkResult(err);
#ifdef IMGUI_IMPL_VULKAN_USE_VOLK
    volkLoadInstance(g_Instance);
#endif

    // Setup the debug report callback
#ifdef APP_USE_VULKAN_DEBUG_REPORT
    auto f_vkCreateDebugReportCallbackEXT =
        (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(
            g_Instance, "vkCreateDebugReportCallbackEXT");
    GB_ASSERT(f_vkCreateDebugReportCallbackEXT);
    VkDebugReportCallbackCreateInfoEXT debug_report_ci = {};
    debug_report_ci.sType =
        VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    debug_report_ci.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT |
                            VK_DEBUG_REPORT_WARNING_BIT_EXT |
                            VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
    debug_report_ci.pfnCallback = vkDebugCallback;
    debug_report_ci.pUserData = nullptr;
    err = f_vkCreateDebugReportCallbackEXT(g_Instance, &debug_report_ci,
                                           g_Allocator, &g_DebugReport);
    checkVkResult(err);
#endif
}

void Application::createLogicalDevice()
{
    VkResult err;

    std::vector<const char*> deviceExtensions;
    deviceExtensions.push_back("VK_KHR_swapchain");

    // Enumerate physical device extension
    uint32_t properties_count;
    std::vector<VkExtensionProperties> properties;
    vkEnumerateDeviceExtensionProperties(g_PhysicalDevice, nullptr,
                                         &properties_count, nullptr);
    properties.resize(properties_count);
    vkEnumerateDeviceExtensionProperties(g_PhysicalDevice, nullptr,
                                         &properties_count, properties.data());
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
    if (isExtensionAvailable(properties,
                             VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME))
        device_extensions.push_back(VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME);
#endif

    const float queuePriority[] = {1.0f};
    VkDeviceQueueCreateInfo queueCreateInfo[1] = {};
    queueCreateInfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo[0].queueFamilyIndex = g_QueueFamily;
    queueCreateInfo[0].queueCount = 1;
    queueCreateInfo[0].pQueuePriorities = queuePriority;

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount =
        sizeof(queueCreateInfo) / sizeof(queueCreateInfo[0]);
    createInfo.pQueueCreateInfos = queueCreateInfo;
    createInfo.enabledExtensionCount = (uint32_t) deviceExtensions.size();
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();
    err = vkCreateDevice(g_PhysicalDevice, &createInfo, g_Allocator, &g_Device);
    checkVkResult(err);

    vkGetDeviceQueue(g_Device, g_QueueFamily, 0, &g_Queue);
}

void Application::createDescriptorPool()
{
    VkResult err;

    VkDescriptorPoolSize pool_sizes[] = {
        {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
         IMGUI_IMPL_VULKAN_MINIMUM_IMAGE_SAMPLER_POOL_SIZE},
    };

    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.maxSets = 0;
    for (VkDescriptorPoolSize& pool_size : pool_sizes) {
        pool_info.maxSets += pool_size.descriptorCount;
    }
    pool_info.poolSizeCount = (uint32_t) IM_ARRAYSIZE(pool_sizes);
    pool_info.pPoolSizes = pool_sizes;

    err = vkCreateDescriptorPool(g_Device, &pool_info, g_Allocator,
                                 &g_DescriptorPool);
    checkVkResult(err);
}

void Application::setupVulkanWindow(ImGui_ImplVulkanH_Window* wd,
                                    VkSurfaceKHR surface, int width, int height)
{
    wd->Surface = surface;

    // Check for WSI support
    VkBool32 res;
    vkGetPhysicalDeviceSurfaceSupportKHR(g_PhysicalDevice, g_QueueFamily,
                                         wd->Surface, &res);
    GB_ASSERT(res == VK_TRUE, "No WSI support on physical device!");

    // Select Surface Format
    const VkFormat requestSurfaceImageFormat[] = {
        VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM,
        VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM};
    const VkColorSpaceKHR requestSurfaceColorSpace =
        VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(
        g_PhysicalDevice, wd->Surface, requestSurfaceImageFormat,
        (size_t) IM_ARRAYSIZE(requestSurfaceImageFormat),
        requestSurfaceColorSpace);

    // Select Present Mode
#ifdef APP_USE_UNLIMITED_FRAME_RATE
    VkPresentModeKHR present_modes[] = {VK_PRESENT_MODE_MAILBOX_KHR,
                                        VK_PRESENT_MODE_IMMEDIATE_KHR,
                                        VK_PRESENT_MODE_FIFO_KHR};
#else
    VkPresentModeKHR present_modes[] = {VK_PRESENT_MODE_FIFO_KHR};
#endif

    wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(
        g_PhysicalDevice, wd->Surface, &present_modes[0],
        sizeof(present_modes) / sizeof(present_modes[0]));

    // Create SwapChain, RenderPass, Framebuffer, etc.
    GB_ASSERT(g_MinImageCount >= 2, "MinImageCount must be at least 2 !");
    ImGui_ImplVulkanH_CreateOrResizeWindow(g_Instance, g_PhysicalDevice,
                                           g_Device, wd, g_QueueFamily, nullptr,
                                           width, height, g_MinImageCount);
}

void Application::cleanupVulkan()
{
    //     // Free resources in queue
    //     for (auto& queue : s_ResourceFreeQueue) {
    //         for (auto& func : queue) {
    //             func();
    //         }
    //     }
    //     s_ResourceFreeQueue.clear();

    vkDestroyDescriptorPool(g_Device, g_DescriptorPool, g_Allocator);

#ifdef APP_USE_VULKAN_DEBUG_REPORT
    // Remove the debug report callback
    auto f_vkDestroyDebugReportCallbackEXT =
        (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(
            g_Instance, "vkDestroyDebugReportCallbackEXT");
    f_vkDestroyDebugReportCallbackEXT(g_Instance, g_DebugReport, g_Allocator);
#endif  // APP_USE_VULKAN_DEBUG_REPORT

    vkDestroyDevice(g_Device, g_Allocator);
    vkDestroyInstance(g_Instance, g_Allocator);
}

//     VkInstance Application::GetInstance()
//     {
//         return g_Instance;
//     }

//     VkPhysicalDevice Application::GetPhysicalDevice()
//     {
//         return g_PhysicalDevice;
//     }

//     VkDevice Application::GetDevice()
//     {
//         return g_Device;
//     }

//     VkCommandBuffer Application::GetCommandBuffer()
//     {
//         VkResult result;
//         ImGui_ImplVulkanH_Window* wd = &g_WindowData;

//         VkCommandPool commandPool =
//         wd->Frames[wd->FrameIndex].CommandPool; VkCommandBuffer&
//         commandBuffer = s_CommandBuffers[wd->FrameIndex].emplace_back();

//         // Allocate command buffer from pool
//         {
//             VkCommandBufferAllocateInfo info{};
//             info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
//             info.commandPool = commandPool;
//             info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
//             info.commandBufferCount = 1;

//             result = vkAllocateCommandBuffers(g_Device, &info,
//             &commandBuffer); GB_ASSERT(result == VK_SUCCESS, "Cannot
//             allocate command buffer!");
//         }

//         // Begin command buffer
//         {
//             VkCommandBufferBeginInfo info = {};
//             info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//             info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

//             result = vkBeginCommandBuffer(commandBuffer, &info);
//             GB_ASSERT(result == VK_SUCCESS, "Failed to begin command
//             buffer!");
//         }
//         return commandBuffer;
//     }

//     void Application::FlushCommandBuffer(VkCommandBuffer commandBuffer)
//     {
//         VkResult result;
//         const uint64_t timeout = 100000000000;

//         // End command buffer
//         VkSubmitInfo endInfo = {};
//         endInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
//         endInfo.commandBufferCount = 1;
//         endInfo.pCommandBuffers = &commandBuffer;

//         result = vkEndCommandBuffer(commandBuffer);
//         GB_ASSERT(result == VK_SUCCESS, "Cannot end command buffer!");

//         VkFence fence;

//         // Create fence to ensure that the command buffer has finished
//         executing VkFenceCreateInfo fenceCreateInfo = {};
//         fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
//         fenceCreateInfo.flags = 0;
//         result = vkCreateFence(g_Device, &fenceCreateInfo, nullptr,
//         &fence); GB_ASSERT(result == VK_SUCCESS, "Cannot create fence!");

//         // Submit to queue
//         result = vkQueueSubmit(g_Queue, 1, &endInfo, fence);
//         GB_ASSERT(result == VK_SUCCESS, "Cannot submit to queue!");

//         result = vkWaitForFences(g_Device, 1, &fence, VK_TRUE, timeout);
//         GB_ASSERT(result == VK_SUCCESS, "Fence is not ready!");

//         // Destroy fence
//         vkDestroyFence(g_Device, fence, nullptr);
//     }

//     void Application::SubmitResourceFree(std::function<void()>&& func)
//     {
//         s_ResourceFreeQueue[s_CurrentFrameIndex].emplace_back(func);
//     }
}  // namespace grafix