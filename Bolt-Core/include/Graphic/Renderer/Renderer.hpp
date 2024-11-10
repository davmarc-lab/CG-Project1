#pragma once

#include "../Buffer/VertexArray.hpp"

namespace bolt {
	class Renderer {
	public:
		Renderer() = default;

		virtual ~Renderer() = default;

		virtual void onAttach();

		virtual void onDetach();

		virtual void drawArrays(const VertexArray &vao, const u32 &mode, const i32 &first, const i32 &count);

		virtual void drawElements(const VertexArray &vao, const u32 &mode, const i32 &count, const u32 &type, const void *indices = nullptr);

		virtual void drawArraysLines(const VertexArray &vao, const i32 &count, const i32 &first = 0);

		virtual void drawElementsLines(const VertexArray &vao, const i32 &count);

		virtual void drawArraysTriangles(const VertexArray &vao, const i32 &count, const i32 &first = 0);

		virtual void drawArraysTriangleFan(const VertexArray &vao, const i32 &count, const i32 &first = 0);

		virtual void drawElementsTriangles(const VertexArray &vao, const i32 &count);
	};
} // namespace bolt
