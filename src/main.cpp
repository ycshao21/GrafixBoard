#include <array>
#include <iostream>

#include <grafixcore/algorithm/bsplincurve.hpp>
#include <grafixcore/math/vec_add.hpp>

auto main() -> int
{
    auto printVec = [](const auto& vec) -> void {
        for (const auto& v : vec) {
            std::cout << v << ", ";
        }
        std::cout << "\n";
    };

    auto a = std::array<float, 3>{1, 2, 3};
    auto b = std::array<float, 3>{4, 5, 6};
    auto c = std::array<float, 3>{0, 0, 0};

    printVec(a);
    printVec(b);

    std::cout << __cplusplus << "\n";

    grafixboard::launchVecAdd(a.data(), b.data(), c.data(), int(a.size()));
    printVec(c);

    auto proxy = pro::make_proxy<grafixboard::algo::Drawable,
                                 grafixboard::algo::BSplineCurve>();

    proxy->draw();
    return 0;
}