#pragma once

#include "Core.hpp"

// #include "../../dependencies/spdlog/include/spdlog/spdlog.h"
#include "Utils.hpp"

namespace bolt {
	class Log {
	private:
		// static Shared<spdlog::logger> s_coreLogger;
		// static Shared<spdlog::logger> s_clientLogger;

	public:
		Log() = default;

		~Log() = default;

		static void init();

		// static Shared<spdlog::logger>& getCoreLogger() { return s_coreLogger; }
		//
		// static Shared<spdlog::logger>& getClientLogger() { return s_clientLogger; }
	};
} // namespace Bolt

// #define BT_INFO_CORE(...) ::Bolt::Log::getCoreLogger()->info(__VA_ARGS__)
// #define BT_DEBUG_CORE(...) ::Bolt::Log::getCoreLogger()->debug(__VA_ARGS__)
// #define BT_WARN_CORE(...) ::Bolt::Log::getCoreLogger()->warn(__VA_ARGS__)
// #define BT_ERROR_CORE(...) ::Bolt::Log::getCoreLogger()->error(__VA_ARGS__)
// #define BT_CRITICAL_CORE(...) ::Bolt::Log::getCoreLogger()->critical(__VA_ARGS__)

// #define BT_INFO(...) ::Bolt::Log::getClientLogger()->info(__VA_ARGS__)
// #define BT_DEBUG(...) ::Bolt::Log::getClientLogger()->debug(__VA_ARGS__)
// #define BT_WARN(...) ::Bolt::Log::getClientLogger()->warn(__VA_ARGS__)
// #define BT_ERROR(...) ::Bolt::Log::getClientLogger()->error(__VA_ARGS__)
// #define BT_CRITICAL(...) ::Bolt::Log::getClientLogger()->critical(__VA_ARGS__)
