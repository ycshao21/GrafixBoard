#pragma once

#include <grafixcore/common/type_traits.hpp>
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

    template <class F, bool IS_DIRECT, class R>
    struct accessor
    {
        friend auto name_of(
            const std::conditional_t<IS_DIRECT, pro::proxy<F>,
                                     pro::proxy_indirect_accessor<F>>&
                self) noexcept -> std::string_view
        {
            const AlgoReflector& refl =
                pro::proxy_reflect<IS_DIRECT, R>(pro::access_proxy<F>(self));
            return refl.name;
        }
    };

    std::string_view name;
};

// clang-format off
struct Drawable
    : pro::facade_builder
    ::add_convention<AlgoMemDraw, void()>
    ::add_convention<AlgoMemSetParams, void(const std::vector<std::byte>&)>
    ::add_reflection<AlgoReflector>
    ::add_indirect_reflection<AlgoReflector>
    ::build
{
};
// clang-format on

}  // namespace grafix::algo