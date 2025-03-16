#include <array>
#include <iostream>

#include <grafixcore/algorithm/bsplinecurve.hpp>
#include <grafixcore/common/common.hpp>

#include <grafixcore/entry.hpp>
#include <grafixcore/grafixcore.hpp>

class TestApp : public grafix::Application
{
public:
    TestApp(const grafix::ApplicationConfig& appSpec)
        : grafix::Application(appSpec)
    {
        auto proxy = pro::make_proxy<grafix::algo::Drawable,
                                     grafix::algo::BSplineCurve>();

        proxy->draw();

        GB_INFO("{}", name_of(*proxy));
    }

    ~TestApp() = default;
};

auto grafix::create_application() -> grafix::Application*
{
    grafix::ApplicationConfig config;
    config.name = "TestApp";
    config.width = 1280;
    config.height = 720;
    // config.renderer_api = grafix::RendererAPI::Vulkan;
    config.target_frame_rate = 60.0F;

    return new TestApp(config);
}