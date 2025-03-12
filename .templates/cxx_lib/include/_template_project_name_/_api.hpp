#pragma once

#if defined(_WIN32) || defined(__WIN32__)
    #if defined(_template_project_name__EXPORT)
        #define _template_project_name__API __declspec(dllexport)
    #elif defined(_template_project_name__IMPORT)
        #define _template_project_name__API __declspec(dllimport)
    #else
        #define _template_project_name__API
    #endif
#else
    #if defined(_template_project_name__EXPORT)
        #define _template_project_name__API                                    \
            __attribute__((visibility("default")))
    #else
        #define _template_project_name__API
    #endif
#endif