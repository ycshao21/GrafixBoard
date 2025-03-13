#pragma once

#if defined(_WIN32) || defined(__WIN32__)
    #if defined(GRAFIXBOARD_EXPORT)
        #define GRAFIXBOARD_API __declspec(dllexport)
    #elif defined(GRAFIXBOARD_IMPORT)
        #define GRAFIXBOARD_API __declspec(dllimport)
    #else
        #define GRAFIXBOARD_API
    #endif
#else
    #if defined(GRAFIXBOARD_EXPORT)
        #define GRAFIXBOARD_API                                    \
            __attribute__((visibility("default")))
    #else
        #define GRAFIXBOARD_API
    #endif
#endif