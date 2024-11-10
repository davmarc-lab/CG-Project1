#include "../../include/Core/Log.hpp"

// #include "../../dependencies/spdlog/include/spdlog/sinks/stdout_color_sinks.h"

namespace bolt {
	// Shared<spdlog::logger> Log::s_coreLogger;
	// Shared<spdlog::logger> Log::s_clientLogger;

	void Log::init() {
		/* spdlog::set_pattern("%^[%T] %n: %v%$");

		s_coreLogger = spdlog::stdout_color_mt("Bolt");
		BT_INFO_CORE("Logger created");
		s_clientLogger = spdlog::stdout_color_mt("App");
		BT_INFO("Logger created"); */
	}
}
