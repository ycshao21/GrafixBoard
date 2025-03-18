#pragma once

#include "core/application.hpp"
#include "utils/log.hpp"

namespace grafix
{
inline auto grafixMain([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
    -> int
{
    Log::init();

    Application* app = createApplication();
    app->run();
    delete app;

    Log::shutdown();
    return 0;
}
}  // namespace grafix

auto main(int argc, char** argv) -> int
{
    return grafix::grafixMain(argc, argv);
}