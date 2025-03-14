#pragma once

#include <proxy/proxy.h>

namespace grafixboard::algo
{

PRO_DEF_MEM_DISPATCH(AlgoMemDraw, draw);
PRO_DEF_MEM_DISPATCH(AlgoMemSetParams, setParams);

// clang-format off
struct Drawable
    : pro::facade_builder
    ::add_convention<AlgoMemDraw, void()>
    ::add_convention<AlgoMemSetParams, void(const std::vector<std::byte>&)>
    ::support_copy<pro::constraint_level::nontrivial>
    ::build
{
};
// clang-format on

}  // namespace grafixboard::algo