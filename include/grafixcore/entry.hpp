#pragma once

#include "core/application.hpp"
#include "core/log.hpp"

namespace grafix
{
inline auto grafix_main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
    -> int
{
    Log::init();

    Application* app = create_application();
    app->run();
    delete app;

    Log::shutdown();
    return 0;
}
}  // namespace grafix

auto main(int argc, char** argv) -> int
{
    return grafix::grafix_main(argc, argv);
}