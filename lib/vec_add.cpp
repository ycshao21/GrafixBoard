#include "grafixcore/pch.hpp"

#include <cstdint>
#include <cstdio>

#include "grafixcore/math/vec_add.hpp"
#include "grafixcore/utils/address.hpp"

namespace grafixboard
{

void launchVecAdd(const float* a, const float* b, float* c, const int n)
{
    ::printf("Hello World from CPU!\n");
    ::printf("Vector size: %d\n", n);

    for (int i = 0; i < n; ++i) {
        auto offset = computeOffset<std::size_t>(1, 2, 3, 4, 5, 6);
        ::printf("Offset: %d\n", std::uint32_t(offset));
        c[i] = a[i] + b[i];
    }
}

}  // namespace grafixboard