#pragma once
#include <cstdint>

namespace grafixboard
{
using int8_t = ::std::int8_t;
static_assert(sizeof(int8_t) == 1);
using uint8_t = ::std::uint8_t;
static_assert(sizeof(uint8_t) == 1);

using uint16_t = ::std::uint16_t;
static_assert(sizeof(uint16_t) == 2);
using int16_t = ::std::int16_t;
static_assert(sizeof(int16_t) == 2);

using int32_t = ::std::int32_t;
static_assert(sizeof(int32_t) == 4);
using uint32_t = ::std::uint32_t;
static_assert(sizeof(uint32_t) == 4);

using int64_t = ::std::int64_t;
static_assert(sizeof(int64_t) == 8);
using uint64_t = ::std::uint64_t;
static_assert(sizeof(uint64_t) == 8);

using size_t = ::std::size_t;

using fp32_t = float;
static_assert(sizeof(fp32_t) == 4);
using fp64_t = double;
static_assert(sizeof(fp64_t) == 8);
}  // namespace grafixboard