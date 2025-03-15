#include <grafixcore/algorithm/bsplinecurve.hpp>
#include <grafixcore/common/common.hpp>

void logParams(const grafix::ByteStream& bytes)
{
    auto nCtrlPoints = bytes.read<uint32_t>();
    spdlog::info("Number of control points: {}", nCtrlPoints);

    std::vector<grafix::algo::BSplineCurve::Point> ctrlPoints(nCtrlPoints);
    bytes.read(ctrlPoints.data(), nCtrlPoints);
    for (const auto& p : ctrlPoints) {
        spdlog::info("({}, {})", p[0], p[1]);
    }

    auto order = bytes.read<uint32_t>();
    spdlog::info("Order: {}", order);
}

auto main() -> int
{
    grafix::ByteStream bytes;

    uint32_t nCtrlPoints = 4;
    bytes.write(nCtrlPoints);

    auto ctrlPoints = std::vector<grafix::algo::BSplineCurve::Point>{
        {0.0F, 0.0F}, {1.0F, 1.0F}, {2.0F, 2.0F}, {3.0F, 3.0F}};
    bytes.write(ctrlPoints.data(), ctrlPoints.size());

    uint32_t order = 3;
    bytes.write(order);

    logParams(bytes);
    bytes.resetOffset();

    auto proxy =
        pro::make_proxy<grafix::algo::Drawable, grafix::algo::BSplineCurve>();

    proxy->setParams(bytes);
    proxy->draw();

    return 0;
}