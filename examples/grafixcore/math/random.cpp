#include <grafixcore/math/random.hpp>
#include <spdlog/spdlog.h>

using namespace grafix;

auto main() -> int
{
    spdlog::info("Random Uniform: {}",
                 RandUniform<fp32_t>::generate(0.0F, 1.0F));

    spdlog::info("Random Normal: {}", RandNormal<fp32_t>::generate(0.0, 1.0));

    auto randVec = RandUniform<fp32_t>::generateVec(100, 0.0F, 1.0F);

    auto distVis = DistributionVisualizer<fp32_t>();

    distVis.visualize(randVec, 10, 15, std::cout);

    return 0;
}