#include "grafixcore/utils/log.hpp"

namespace grafix
{
void Log::init()
{
    // Format: [YYYY-mm-ddTHH:MM:SS±ZZZZ] [logger_name/log_level] message
    const char* logPattern = "%^[%Y-%m-%dT%T%z] [%n/%l]%$ %v";

    s_coreLogger = spdlog::stdout_color_mt<spdlog::async_factory>("Core");
    s_coreLogger->set_pattern(logPattern);
    s_coreLogger->set_level(spdlog::level::trace);

    s_clientLogger = spdlog::stdout_color_mt<spdlog::async_factory>("Client");
    s_clientLogger->set_pattern(logPattern);
    s_clientLogger->set_level(spdlog::level::trace);
}

void Log::shutdown()
{
    s_coreLogger.reset();
    s_clientLogger.reset();

    spdlog::drop_all();
    spdlog::shutdown();
}
}  // namespace grafix