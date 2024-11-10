#pragma once

#include <cassert>
#include <chrono>
#include <cstdlib>
#include <mutex>

#include "Utils.hpp"

namespace bolt {
	class Timer final {
	public:
		Timer(Timer& other) = delete;

		void operator=(const Timer& other) = delete;

		/*
		 * Retrieves the instance of the Timer if it's not created.
		 * This function is thread safe using a simple `std::mutex`.
		 *
		 * @return `Timer` unique object.
		 */
		inline static Shared<Timer> instance() {
			std::lock_guard<std::mutex> lock(s_mutex);
			if (s_pointer == nullptr) {
				Shared<Timer> copy(new Timer());
				copy.swap(s_pointer);
			}

			return s_pointer;
		}

		inline void start() {
			this->m_prevTime = std::chrono::system_clock::now();
			this->m_init = true;
		}

		inline void reset() { this->start(); }

		inline f64 getTime() const {
			assert(this->m_init);
			auto now = std::chrono::system_clock::now();
			return std::chrono::duration<f64>(now - this->m_prevTime).count();
		}
		
		inline f32 getTimef() const {
			assert(this->m_init);
			auto now = std::chrono::system_clock::now();
			return std::chrono::duration<f32>(now - this->m_prevTime).count();
		}

		~Timer() = default;

	private:
		Timer() = default;

		inline static Shared<Timer> s_pointer = nullptr;
		inline static std::mutex s_mutex;

		b8 m_init = false;
		std::chrono::time_point<std::chrono::system_clock> m_prevTime{};
	};

	class Timestep final {
	public:
		f32 getTime() const { return static_cast<f32>(Timer::instance()->getTime()); }

		const f32& getTimeStep() const { return this->m_timeStep; }

		Timestep(const f32& timeStep = 1 / 60.f) : m_timeStep(timeStep) {}

		~Timestep() = default;

	private:
		f32 m_timeStep;
	};
}; // namespace bolt
