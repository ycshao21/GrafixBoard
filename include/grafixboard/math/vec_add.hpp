#pragma once

#include "grafixboard/_api.hpp"

namespace grafixboard
{
GRAFIXBOARD_API void launch_vec_add(const float* const a,
                                                const float* const b,
                                                float* const c, const int n);
}  // namespace grafixboard
