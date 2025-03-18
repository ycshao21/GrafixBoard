#pragma once

#include <glm/glm.hpp>
#include <imgui_impl_vulkan.h>

#include "grafixcore/core/layer.hpp"

namespace grafix
{

enum class UIStyle : uint8_t
{
    Dark = 0,
    Light,
    Classic
};

class ImGuiLayer final : public Layer
{
public:
    ImGuiLayer() : Layer("ImGui Layer")
    {
    }

    ~ImGuiLayer()
    {
    }

    void onAttach() override;
    void onDetach() override;

    void onEvent(Event& e) override;

    void blockEvents(bool block)
    {
        m_blockEvents = block;
    }

    static void beginFrame();
    static void endFrame(ImGui_ImplVulkanH_Window* wd);

    static void setThemeColor();

private:
    bool m_blockEvents = true;
};
}  // namespace grafix
