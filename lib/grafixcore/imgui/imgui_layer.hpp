#pragma once

#include "grafixcore/core/layer.hpp"

namespace grafix
{
class ImGuiLayer final : public Layer
{
public:
    ImGuiLayer() : Layer("ImGui Layer")
    {
    }

    ~ImGuiLayer()
    {
    }

    void on_attach() override;
    void on_detach() override;

    // virtual void OnEvent(Event& e) override;

    void begin_frame();
    void end_frame();

    void block_events(bool block)
    {
        m_block_events = block;
    }

    void set_theme_color();

private:
    bool m_block_events = true;
};
}  // namespace grafix
