#include <array>
#include <iostream>

#include <grafixcore/algorithm/attributes.hpp>
#include <grafixcore/algorithm/bsplinecurve.hpp>
#include <grafixcore/common/concepts.hpp>

auto main() -> int
{
    auto proxy =
        pro::make_proxy<grafix::algo::Drawable, grafix::algo::BSplineCurve>();

    static_assert(
        grafix::proxiable<grafix::algo::BSplineCurve, grafix::algo::Drawable>);

    proxy->draw();

    spdlog::info("{}", name_of(*proxy));
    return 0;
}