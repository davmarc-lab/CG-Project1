#pragma once

#include "../Graphic/Renderer/Renderer.hpp"


namespace Bolt {
	class GlRenderer : public Renderer {
	public:
		GlRenderer() = default;

		virtual ~GlRenderer() override = default;

		virtual void onAttach() override;

		static void setPolygonMode();

		virtual void drawArrays(const VertexArray& vao, const u32& mode, const i32& first, const i32& count) override;

		virtual void drawElements(const VertexArray& vao, const u32& mode, const i32& count, const u32& type, const void* indices = nullptr) override;

		virtual void drawArraysLines(const VertexArray& vao, const i32& count, const i32& first = 0) override;

		virtual void drawElementsLines(const VertexArray& vao, const i32& count) override;

		virtual void drawArraysTriangles(const VertexArray& vao, const i32& count, const i32& first = 0) override;

		virtual void drawElementsTriangles(const VertexArray& vao, const i32& count) override;
	};
}
