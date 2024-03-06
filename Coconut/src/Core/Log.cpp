#include "Core/Log.h"

namespace Coconut {
Log::Log() {}

Log::~Log() {}

std::shared_ptr< spdlog::logger > Log::s_CoreLogger;
std::shared_ptr< spdlog::logger > Log::s_ClientLogger;

void Log::init() {
    spdlog::set_pattern(" [%l] [%T] %v ");
    s_CoreLogger = spdlog::stdout_color_mt("Coconut");
    s_CoreLogger->set_level(spdlog::level::level_enum::trace);

    s_ClientLogger = spdlog::stdout_color_mt("APP");
    s_ClientLogger->set_level(spdlog::level::level_enum::trace);
    // CC_CORE_INFO("spdlog initialized!");
}
}  // namespace Coconut