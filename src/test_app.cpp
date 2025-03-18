#include <array>
#include <iostream>

#include <grafixcore/entry.hpp>
#include <grafixcore/grafixcore.hpp>
// TEMP
#include <grafixcore/algorithm/bsplinecurve.hpp>
#include <grafixcore/common/common.hpp>

#include "test_layer.hpp"

class TestApp : public grafix::Application
{
public:
    TestApp(const grafix::ApplicationConfig& config)
        : grafix::Application(config)
    {
        auto proxy = pro::make_proxy<grafix::algo::Drawable,
                                     grafix::algo::BSplineCurve>();

        proxy->draw();
        GB_INFO("{}", name_of(*proxy));

        pushLayer(new TestLayer());
    }

    ~TestApp() = default;
};

auto grafix::createApplication() -> grafix::Application*
{
    grafix::ApplicationConfig config;
    config.name = "TestApp";
    config.width = 1280;
    config.height = 720;
    // config.renderer_api = grafix::RendererAPI::Vulkan;
    config.targetFrameRate = 60.0f;

    return new TestApp(config);
}