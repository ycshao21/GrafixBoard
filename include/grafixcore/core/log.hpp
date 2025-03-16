#pragma once

#include <memory>

#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace grafix
{

class Log
{
public:
    // enum class type : uint8_t
    // {
    //     Core,
    //     Client
    // };

    // enum class level : uint8_t
    // {
    //     Trace,
    //     Debug,
    //     Info,
    //     Warn,
    //     Error,
    //     Critical
    // };

    static void init();
    static void shutdown();

    static auto get_core_logger() -> std::shared_ptr<spdlog::logger>&
    {
        return m_CoreLogger;
    }
    static auto get_client_logger() -> std::shared_ptr<spdlog::logger>&
    {
        return m_ClientLogger;
    }

private:
    inline static std::shared_ptr<spdlog::logger> m_CoreLogger = nullptr;
    inline static std::shared_ptr<spdlog::logger> m_ClientLogger = nullptr;
};

#define ENABLE_LOGGING

#ifdef ENABLE_LOGGING

// -------------- Logging Macros --------------

// Core logging

    #define GB_CORE_DEBUG(...)                                                 \
        ::grafix::Log::get_core_logger()->debug(__VA_ARGS__)
    #define GB_CORE_TRACE(...)                                                 \
        ::grafix::Log::get_core_logger()->trace(__VA_ARGS__)
    #define GB_CORE_INFO(...)                                                  \
        ::grafix::Log::get_core_logger()->info(__VA_ARGS__)
    #define GB_CORE_WARN(...)                                                  \
        ::grafix::Log::get_core_logger()->warn(__VA_ARGS__)
    #define GB_CORE_ERROR(...)                                                 \
        ::grafix::Log::get_core_logger()->error(__VA_ARGS__)
    #define GB_CORE_CRITICAL(...)                                              \
        ::grafix::Log::get_core_logger()->critical(__VA_ARGS__)

// Client logging

    #define GB_DEBUG(...) ::grafix::Log::get_client_logger()->debug(__VA_ARGS__)
    #define GB_TRACE(...) ::grafix::Log::get_client_logger()->trace(__VA_ARGS__)
    #define GB_INFO(...) ::grafix::Log::get_client_logger()->info(__VA_ARGS__)
    #define GB_WARN(...) ::grafix::Log::get_client_logger()->warn(__VA_ARGS__)
    #define GB_ERROR(...) ::grafix::Log::get_client_logger()->error(__VA_ARGS__)
    #define GB_CRITICAL(...)                                                   \
        ::grafix::Log::get_client_logger()->critical(__VA_ARGS__)

#else

    #define GB_CORE_DEBUG(...)
    #define GB_CORE_TRACE(...)
    #define GB_CORE_INFO(...)
    #define GB_CORE_WARN(...)
    #define GB_CORE_ERROR(...)
    #define GB_CORE_CRITICAL(...)

    #define GB_DEBUG(...)
    #define GB_TRACE(...)
    #define GB_INFO(...)
    #define GB_WARN(...)
    #define GB_ERROR(...)
    #define GB_CRITICAL(...)

#endif

}  // namespace grafix
