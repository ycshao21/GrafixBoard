#include "grafixcore/pch.hpp"

#include "grafixcore/algorithm/bsplinecurve.hpp"

namespace grafix::algo
{
class BSplineCurve::Impl
{
public:
    void setParams(const ByteStream& bytes)
    {
        m_numCtrlPoints = bytes.read<uint32_t>();
        m_ctrlPoints.resize(m_numCtrlPoints);
        bytes.read(m_ctrlPoints.data(), m_numCtrlPoints);
        m_order = bytes.read<uint32_t>();
        // [BUG] This might be wrong. Maybe we should use a function pointer?
        m_putPointFunc = bytes.read<std::function<void(const Point&)>>();

        generateKnots();
        generateWeights();
    }

    void draw()
    {
        for (float u = 0; u <= 1.0; u += m_drawStep) {
            float denominator = 0.0F;
            Point p = {0.0F, 0.0F};
            for (size_t i = 0; i < m_ctrlPoints.size(); ++i) {
                float weight =
                    m_weights[i] * basicFunction(i, m_order, u, m_knots);
                denominator += weight;
                p[0] += m_ctrlPoints[i][0] * weight;
                p[1] += m_ctrlPoints[i][1] * weight;
            }
            if (denominator != 0.0F) {
                p[0] /= denominator;
                p[1] /= denominator;
            }
            m_putPointFunc(p);
        }
    }

    void generateKnots()
    {
        size_t num = m_ctrlPoints.size() + m_order;
        if (m_knots.size() != num) {
            m_knots.clear();
            // Generate a new random knot vector
            m_knots.resize(num, 0.0F);
            for (size_t i = num - 1; i >= num - m_order; --i) {
                m_knots.at(i) = 1;
            }
            for (size_t i = m_order; i < num - m_order; ++i) {
                // Make sure `m_knots[m_order:num-m_order-1]` is strictly
                // increasing
                auto r = RandUniform<fp32_t>::generate(
                             std::numeric_limits<fp32_t>::epsilon() * 2.0F,
                             1.0F - m_knots[i - 1]) *
                         0.5;
                m_knots[i] = r + m_knots[i - 1];
            }
        }
    }

    void generateWeights()
    {
        size_t num = m_ctrlPoints.size();
        if (m_weights.size() != num) {
            m_weights.clear();
            m_weights.resize(num);
            for (auto& w : m_weights) {
                w = RandUniform<float>::generate(15.F, 25.F);
            }
            float sum =
                std::accumulate(m_weights.begin(), m_weights.end(), 0.0F);
            for (auto& w : m_weights) {
                w /= sum;
            }
        }
    }

    static auto basicFunction(size_t i, int32_t order, fp32_t t,
                              std::vector<fp32_t>& knots) -> fp32_t
    {
        if (order == 1) {
            if (t >= knots[i] && t <= knots[i + 1ULL]) {
                return 1.0F;
            }
            return 0.0F;
        }
        // Or k > 1, then the BSpline is a piecewise polynomial function
        fp32_t deltaA = knots[i + order - 1ULL] - knots[i];
        fp32_t deltaB = knots[i + order] - knots[i + 1ULL];
        fp32_t a = (t - knots[i]) / (deltaA == 0.0F ? 1.0F : deltaA);
        fp32_t b = (knots[i + order] - t) / (deltaB == 0.0F ? 1.0F : deltaB);
        return a * basicFunction(i, order - 1, t, knots) +
               b * basicFunction(i + 1ULL, order - 1, t, knots);
    }

private:
    // Parameters for BSplineCurve
    uint32_t m_numCtrlPoints = 0;
    std::vector<Point> m_ctrlPoints;
    uint32_t m_order = 3ULL;
    fp32_t m_drawStep = 1e-4F;
    std::function<void(const Point&)> m_putPointFunc = [](const Point&) {};

    std::vector<fp32_t> m_weights;
    std::vector<fp32_t> m_knots;
};

BSplineCurve::BSplineCurve() : pImpl(std::make_unique<Impl>()) {};

BSplineCurve::~BSplineCurve() = default;

BSplineCurve::BSplineCurve(const BSplineCurve& other)
    : pImpl(std::make_unique<Impl>(*other.pImpl))
{
}

BSplineCurve::BSplineCurve(BSplineCurve&& other) noexcept = default;

void BSplineCurve::setParams(const ByteStream& bytes)
{
    pImpl->setParams(bytes);
}

void BSplineCurve::draw()
{
    pImpl->draw();
}

}  // namespace grafix::algo