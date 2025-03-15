#include <array>
#include <iostream>

#include <grafixcore/algorithm/bsplinecurve.hpp>
#include <grafixcore/common/common.hpp>

auto main() -> int
{
    auto proxy =
        pro::make_proxy<grafix::algo::Drawable, grafix::algo::BSplineCurve>();

    proxy->draw();

    spdlog::info("{}", name_of(*proxy));
    return 0;
}