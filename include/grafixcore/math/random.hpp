#pragma once
#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <memory>
#include <numeric>
#include <random>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <type_traits>
#include <vector>

#include "grafixcore/common/type_traits.hpp"

namespace grafix
{

template <class Derived>
class RandBase
{
public:
    using ValTy = grafix::type_traits::GetInnerType_t<Derived>;

    explicit RandBase() = default;
    auto operator=(const RandBase&) -> RandBase& = delete;

    template <typename... Args>
    static void setParams(Args&&... args)
    {
        Derived::__setParamsImpl(std::forward<Args>(args)...);
    }

    template <typename... Args>
    static auto generate(Args&&... args) -> ValTy
    {
        return Derived::__generateImpl(std::forward<Args>(args)...);
    }

    template <typename... Args>
    static auto generateVec(std::size_t size, Args&&... args)
        -> std::vector<ValTy>
    {
        std::vector<ValTy> vec;
        while ((size--) != 0U) {
            vec.push_back(Derived::__generateImpl(std::forward<Args>(args)...));
        }
        return vec;
    }
};

template <class _ValTy>
    requires std::is_arithmetic_v<_ValTy>
class RandUniform : public RandBase<RandUniform<_ValTy>>
{
public:
    explicit RandUniform() = default;
    auto operator()(const RandUniform&) -> RandUniform& = delete;

    static void __setParamsImpl(double min, double max)
    {
        if (m_distribution->min() != min || m_distribution->max() != max) {
            m_distribution =
                std::make_shared<std::uniform_real_distribution<double>>(min,
                                                                         max);
        }
    }

    static auto __generateImpl() -> _ValTy
    {
        return static_cast<_ValTy>(m_distribution->operator()(m_engine));
    }

    static auto __generateImpl(double min, double max) -> _ValTy
    {
        __setParamsImpl(min, max);
        return static_cast<_ValTy>(m_distribution->operator()(m_engine));
    }

private:
    static std::random_device m_rd;
    static thread_local std::default_random_engine m_engine;
    static std::shared_ptr<std::uniform_real_distribution<double>>
        m_distribution;
};

template <class _ValTy>
    requires std::is_arithmetic_v<_ValTy>
std::random_device grafix::RandUniform<_ValTy>::m_rd{};

template <class _ValTy>
    requires std::is_arithmetic_v<_ValTy>
thread_local std::default_random_engine RandUniform<_ValTy>::m_engine{m_rd()};

template <class _ValTy>
    requires std::is_arithmetic_v<_ValTy>
std::shared_ptr<std::uniform_real_distribution<double>>
    RandUniform<_ValTy>::m_distribution{
        std::make_shared<std::uniform_real_distribution<double>>()};

template <class _ValTy>
    requires std::is_arithmetic_v<_ValTy>
class RandNormal : public RandBase<RandNormal<_ValTy>>
{
public:
    explicit RandNormal() = default;
    auto operator()(const RandNormal&) -> RandNormal& = delete;

    static void __setParamsImpl(double mean, double stddev)
    {
        if (m_distribution->mean() != mean ||
            m_distribution->stddev() != stddev) {
            m_distribution = std::make_shared<std::normal_distribution<double>>(
                mean, stddev);
        }
    }

    static auto __generateImpl() -> _ValTy
    {
        return static_cast<_ValTy>(m_distribution->operator()(m_engine));
    }

    static auto __generateImpl(double mean, double stddev) -> _ValTy
    {
        __setParamsImpl(mean, stddev);
        return static_cast<_ValTy>(m_distribution->operator()(m_engine));
    }

private:
    static std::random_device m_rd;
    static thread_local std::default_random_engine m_engine;
    static std::shared_ptr<std::normal_distribution<double>> m_distribution;
};

template <class _ValTy>
    requires std::is_arithmetic_v<_ValTy>
std::random_device RandNormal<_ValTy>::m_rd{};

template <class _ValTy>
    requires std::is_arithmetic_v<_ValTy>
thread_local std::default_random_engine RandNormal<_ValTy>::m_engine{m_rd()};

template <class _ValTy>
    requires std::is_arithmetic_v<_ValTy>
std::shared_ptr<std::normal_distribution<double>>
    RandNormal<_ValTy>::m_distribution{
        std::make_shared<std::normal_distribution<double>>()};

template <class _ValTy>
    requires std::is_arithmetic_v<_ValTy>
class DistributionVisualizer
{
public:
    explicit DistributionVisualizer() = default;
    auto operator=(const DistributionVisualizer&)
        -> DistributionVisualizer& = delete;

    void visualize(const std::vector<_ValTy>& randVec,
                   const std::size_t kBinNum = 10,
                   const std::size_t kMaxStarNum = 15,
                   std::ostream& os = std::cout)
    {
        if constexpr (!std::is_arithmetic_v<_ValTy>) {
            m_logger->error("The type of the elements in the vector is not "
                            "supported by the visualizer. Skip visualization.");
            m_logger->info("You may need to specialize the template class "
                           "yutils::DistributionVisualizer for the type of the "
                           "elements in the vector.");
        } else {
            if (randVec.empty()) {
                return;
            }
            _ValTy minElem =
                *(std::min_element(randVec.begin(), randVec.end()));
            _ValTy maxElem =
                *(std::max_element(randVec.begin(), randVec.end()));
            _ValTy range = maxElem - minElem;

            if (range == 0) {
                os << spdlog::fmt_lib::format("All the elements are: {}\n",
                                              maxElem);
                return;
            }

            m_logger->info("Min: {} | Max: {}", minElem, maxElem);

            double average =
                std::accumulate(randVec.begin(), randVec.end(), 0.0) /
                randVec.size();
            std::vector<std::size_t> bins(kBinNum);

            os << spdlog::fmt_lib::format("Min: {} | Max: {} | Average: {}\n",
                                          minElem, maxElem, average);

            for (const auto& val : randVec) {
                auto bin = static_cast<std::size_t>(double(val - minElem) /
                                                    range * kBinNum);
                if (bin == bins.size()) {
                    bin -= 1;
                }
                ++bins[bin];
            }
            std::size_t maxS = *(std::ranges::max_element(bins));
            double resizer = double(maxS) / kMaxStarNum;
            for (auto& val : bins) {
                val = (std::size_t) ceil(val / resizer);
            }
            for (std::size_t i = 0; i < bins.size(); ++i) {
                os << spdlog::fmt_lib::format("{:>3}: ", i);
                for (std::size_t j = 0; j < bins[i]; ++j) {
                    os << "*";
                }
                os << "\n";
            }

            os << std::flush;
        }
    }

private:
    std::shared_ptr<spdlog::logger> m_logger =
        spdlog::stdout_color_mt("grafix::DistributionVisualizer");
};

}  // namespace grafix