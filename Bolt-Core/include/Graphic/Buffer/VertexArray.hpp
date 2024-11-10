#pragma once

#include "VertexBuffer.hpp"

#include "../../Core/Utils.hpp"

namespace bolt {
	class VertexArray {
	protected:
		u32 m_id;

	public:
		VertexArray() = default;

		virtual ~VertexArray() = default;

		virtual void onAttach();

		virtual void onDetach();

		virtual void bind() const;

		virtual void unbind() const;

		virtual void linkVertexBuffer(const VertexBuffer& vbo, const u32& layout, const i32& size, const u32& type, const i32& stride, void* offset, const b8& normalize = false) const;

		virtual void linkAttribFast(u32 layout, i32 size, u32 type, b8 normalize, i32 stride, void* offset) const;
	};
} // namespace Bolt
