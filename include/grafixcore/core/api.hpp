#pragma once

#if defined(_WIN32) || defined(__WIN32__)
    #if defined(GRAFIXCORE_EXPORT)
        #define GRAFIXCORE_API __declspec(dllexport)
    #elif defined(GRAFIXCORE_IMPORT)
        #define GRAFIXCORE_API __declspec(dllimport)
    #else
        #define GRAFIXCORE_API
    #endif
#else
    #if defined(GRAFIXCORE_EXPORT)
        #define GRAFIXCORE_API __attribute__((visibility("default")))
    #else
        #define GRAFIXCORE_API
    #endif
#endif