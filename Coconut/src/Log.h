#pragma once

#include <memory>

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

namespace Coconut {
class Log {
    Log();
    ~Log();

public:
    static void init();

    inline static std::shared_ptr< spdlog::logger >& GetCoreLogger() {
        return s_CoreLogger;
    }

    inline static std::shared_ptr< spdlog::logger >& GetClientLogger() {
        return s_ClientLogger;
    }

private:
    static std::shared_ptr< spdlog::logger > s_CoreLogger;
    static std::shared_ptr< spdlog::logger > s_ClientLogger;
};
}  // namespace Coconut

// Core log macros
#define CC_CORE_CRITICAL(...) ::Coconut::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define CC_CORE_ERROR(...) ::Coconut::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CC_CORE_WARN(...) ::Coconut::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CC_CORE_INFO(...) ::Coconut::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CC_CORE_TRACE(...) ::Coconut::Log::GetCoreLogger()->trace(__VA_ARGS__)

// Clinet log macros
#define CC_CRITICAL(...) ::Coconut::Log::GetClientLogger()->critical(__VA_ARGS__)
#define CC_ERROR(...) ::Coconut::Log::GetClientLogger()->error(__VA_ARGS__)
#define CC_WARN(...) ::Coconut::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CC_INFO(...) ::Coconut::Log::GetClientLogger()->info(__VA_ARGS__)
#define CC_TRACE(...) ::Coconut::Log::GetClientLogger()->trace(__VA_ARGS__)