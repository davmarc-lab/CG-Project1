#pragma once

#include "../Graphic.hpp"

namespace ogl {
	/**
	 * @brief Base class of all OpenGL buffer.
	 */
	class Buffer {
	public:
		/**
		 * @brief Retrieves the buffer id
		 *
		 * @return buffer's id
		 */
		inline unsigned int getId() const { return this->m_id; }

		/**
		 * @brief Creates the buffer and sets its id.
		 */
		inline virtual void onAttach() {}
		/**
		 * @brief Destroys the buffer and reset its id.
		 */
		inline virtual void onDetach() {}

		/**
		 * @brief Binds the buffer with the current id.
		 */
		inline virtual void bind() const {}
		/**
		 * @brief Unbinds the buffer.
		 */
		inline virtual void unbind() const {}

		Buffer() = default;
		virtual ~Buffer() = default;

	protected:
		/// buffer id
		unsigned int m_id = 0;
	};
} // namespace ogl
