#pragma once

#include "grafixcore/utils/log.hpp"

#define GB_COMPILER_CLANG
#ifdef GB_COMPILER_CLANG
    #define GB_DEBUGBREAK __builtin_trap()
#else
    #define GB_DEBUGBREAK __debugbreak()
#endif

// #define GB_DEBUG_BUILD
#ifdef GB_DEBUG_BUILD
    #define ENABLE_ASSERT
#endif

#ifdef ENABLE_ASSERT
    #define GB_ASSERT(condition, ...)                                          \
        if (!(condition)) {                                                    \
            GB_CORE_CRITICAL(__VA_ARGS__);                                     \
            GB_DEBUGBREAK;                                                     \
        }
#else
    #define GB_ASSERT(condition, ...) ((void) (condition))
#endif