#pragma once
#include "grafixcore/algorithm/facade.hpp"
#include "grafixcore/common/common.hpp"
#include "grafixcore/math/random.hpp"
#include "grafixcore/utils/bytestream.hpp"

namespace grafix::algo
{

/**
 * @brief Basis spline curve.
 * @see https://en.wikipedia.org/wiki/B-spline
 */
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
     * @todo Improve comments.
     */
    void setParams(const ByteStream& bytes);

    void draw();

private:
    /**
     * @brief Implementation class for BSplineCurve.
     * @see https://en.cppreference.com/w/cpp/language/pimpl
     */
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

static_assert(proxiable<BSplineCurve, Drawable>);

}  // namespace grafix::algo