#pragma once

// #include "Grafix/Events/Event.h"

#include <string>
#include <utility>

namespace grafix
{
class Layer
{
public:
    Layer(std::string name = "Layer") : m_name(std::move(name))
    {
    }
    virtual ~Layer() = default;

    [[nodiscard]] auto get_name() const -> const std::string&
    {
        return m_name;
    }

    virtual void on_attach()
    {
    }

    virtual void on_detach()
    {
    }

    virtual void on_update(float delta_time)
    {
        (void) delta_time;
    }

    virtual void on_ui_render()
    {
    }

    // virtual void on_event(Event& e)
    // {
    // }

protected:
    std::string m_name;
};
}  // namespace grafix
