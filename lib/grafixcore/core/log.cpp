#include "grafixcore/core/log.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace grafix
{
void Log::init()
{
    // Format: [YYYY-mm-ddTHH:MM:SS±ZZZZ] [logger_name/log_level] message
    const char* logPattern = "%^[%Y-%m-%dT%T%z] [%n/%l]%$ %v";

    m_CoreLogger = spdlog::stdout_color_mt<spdlog::async_factory>("Core");
    m_CoreLogger->set_pattern(logPattern);
    m_CoreLogger->set_level(spdlog::level::trace);

    m_ClientLogger = spdlog::stdout_color_mt<spdlog::async_factory>("Client");
    m_ClientLogger->set_pattern(logPattern);
    m_ClientLogger->set_level(spdlog::level::trace);
}

void Log::shutdown()
{
    m_CoreLogger.reset();
    m_ClientLogger.reset();

    spdlog::drop_all();
    spdlog::shutdown();
}
}  // namespace grafix