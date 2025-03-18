#pragma once

#include <string>

#include "event.hpp"

namespace grafix
{
class Layer
{
public:
    Layer(std::string name = "Layer") : m_name(std::move(name))
    {
    }
    virtual ~Layer() = default;

    [[nodiscard]] auto getName() const -> const std::string&
    {
        return m_name;
    }

    virtual void onAttach()
    {
    }

    virtual void onDetach()
    {
    }

    virtual void onUpdate([[maybe_unused]] float delta_time)
    {
    }

    virtual void onUIRender()
    {
    }

    virtual void onEvent([[maybe_unused]] Event& e)
    {
    }

protected:
    std::string m_name;
};
}  // namespace grafix
