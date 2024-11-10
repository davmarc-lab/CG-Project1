#pragma once

#include <vector>
#include "../../Core/Utils.hpp"

namespace bolt {
	class ElementBuffer {
	protected:
		u32 m_id;

	public:
		ElementBuffer() = default;

		virtual ~ElementBuffer();

		virtual void onAttach();

		virtual void onDetach();

		virtual void bind() const;

		virtual void unbind() const;

		/**
		 * @brief Sets up the data for the buffer.
		 * @param indices A pointer to an array of floats to use as data.
		 * @param size The size in bytes of the data.
		 * @param usage The usage type.
		 */
		virtual void setup(const f32* indices, const i64& size, const u32& usage) const;

		/**
		 * @brief Sets up the data for the buffer.
		 * @tparam T Things stored *contiguously* in memory.
		 * @param indices A pointer to an array of Ts to use as data.
		 * @param size The size in bytes of the data.
		 * @param usage The usage type.
		 */
		template <typename T>
		void setup(const T* indices, const i64& size, const u32& usage = 0);

		/**
		 * @brief Sets up the data for the buffer.
		 * @tparam T You might want this to some numeric, vector, etc type for good result,
		 * @param indices A std::vector of something to use as data.
		 * @param usage The usage type.
		 */
		template <typename T>
		void setup(const std::vector<T>& indices, const u32& usage = 0);
	};
} // namespace Bolt
