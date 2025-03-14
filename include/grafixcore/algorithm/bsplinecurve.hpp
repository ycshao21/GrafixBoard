#pragma once
#include "grafixcore/algorithm/attributes.hpp"
#include "grafixcore/common/cxxtypes.hpp"
#include "grafixcore/math/random.hpp"

namespace grafix::algo
{

class BSplineCurve
{
    using Point = std::array<fp32_t, 2>;

public:
    explicit BSplineCurve();
    ~BSplineCurve();
    BSplineCurve(const BSplineCurve&);
    BSplineCurve(BSplineCurve&&) noexcept;

    /**
     * @brief Set the control points of the BSplineCurve.
     *
     * @param bytes Parameters in bytes.
     *        1. uint32_t -> Number of control points.
     *        2. fp32_t[num] -> Control points.
     *        3. uint32_t -> Order of the BSplineCurve.
     *        4. std::function<void(const Point&)> -> Put point function.
     */
    void setParams(const std::vector<std::byte>& bytes);

    void draw();

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

}  // namespace grafix::algo