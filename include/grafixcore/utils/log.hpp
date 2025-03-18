#pragma once

#include <memory>

#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace grafix
{

class GRAFIXCORE_API Log
{
public:
    // enum class Type : uint8_t
    // {
    //     Core = 0,
    //     Client
    // };

    // enum class Level : uint8_t
    // {
    //     Trace = 0,
    //     Debug,
    //     Info,
    //     Warn,
    //     Error,
    //     Critical,
    // };

    static void init();
    static void shutdown();

    static auto getCoreLogger() -> std::shared_ptr<spdlog::logger>
    {
        return s_coreLogger;
    }

    static auto getClientLogger() -> std::shared_ptr<spdlog::logger>
    {
        return s_clientLogger;
    }

private:
    inline static std::shared_ptr<spdlog::logger> s_coreLogger = nullptr;
    inline static std::shared_ptr<spdlog::logger> s_clientLogger = nullptr;
};

#define ENABLE_LOGGING

#ifdef ENABLE_LOGGING

// Core logging

    #define GB_CORE_DEBUG(...)                                                 \
        ::grafix::Log::getCoreLogger()->debug(__VA_ARGS__)
    #define GB_CORE_TRACE(...)                                                 \
        ::grafix::Log::getCoreLogger()->trace(__VA_ARGS__)
    #define GB_CORE_INFO(...) ::grafix::Log::getCoreLogger()->info(__VA_ARGS__)
    #define GB_CORE_WARN(...) ::grafix::Log::getCoreLogger()->warn(__VA_ARGS__)
    #define GB_CORE_ERROR(...)                                                 \
        ::grafix::Log::getCoreLogger()->error(__VA_ARGS__)
    #define GB_CORE_CRITICAL(...)                                              \
        ::grafix::Log::getCoreLogger()->critical(__VA_ARGS__)

// Client logging

    #define GB_DEBUG(...) ::grafix::Log::getClientLogger()->debug(__VA_ARGS__)
    #define GB_TRACE(...) ::grafix::Log::getClientLogger()->trace(__VA_ARGS__)
    #define GB_INFO(...) ::grafix::Log::getClientLogger()->info(__VA_ARGS__)
    #define GB_WARN(...) ::grafix::Log::getClientLogger()->warn(__VA_ARGS__)
    #define GB_ERROR(...) ::grafix::Log::getClientLogger()->error(__VA_ARGS__)
    #define GB_CRITICAL(...)                                                   \
        ::grafix::Log::getClientLogger()->critical(__VA_ARGS__)

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
