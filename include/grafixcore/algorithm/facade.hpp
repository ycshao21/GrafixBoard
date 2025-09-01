#pragma once

#include "grafixcore/common/common.hpp"
#include "grafixcore/utils/bytestream.hpp"
#include <proxy/proxy.h>

namespace grafix::algo
{

PRO_DEF_MEM_DISPATCH(AlgoMemDraw, draw);
PRO_DEF_MEM_DISPATCH(AlgoMemSetParams, setParams);

struct AlgoReflector
{
public:
    template <typename T>
    constexpr explicit AlgoReflector(std::in_place_type_t<T> /*unused*/)
        : name{grafix::type_traits::typeName<T>()}
    {
    }

    template <class P, class R>
    struct accessor
    {
        auto name() const noexcept -> std::string_view
        {
            const AlgoReflector& refl =
                pro::proxy_reflect<R>(static_cast<const P&>(*this));
            return refl.name;
        }
    };

    std::string_view name;
};

// clang-format off
struct Drawable
    : pro::facade_builder
    ::add_convention<AlgoMemDraw, void()>
    ::add_convention<AlgoMemSetParams, void(const ByteStream&)>
    ::add_indirect_reflection<AlgoReflector>
    ::build
{
};
// clang-format on

}  // namespace grafix::algo