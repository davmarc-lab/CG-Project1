#include "../../include/Graphic/Renderer/Renderer.hpp"

#include "../../../Bolt-Graphics/include/glad/glad.h"

#include "../../include/Graphic/Buffer/VertexArray.hpp"

namespace bolt {
	void Renderer::onAttach() {}

	void Renderer::onDetach() {}

	void Renderer::drawArrays(const VertexArray& vao, const u32& mode, const i32& first, const i32& count) {
		vao.bind();
		glDrawArrays(mode, first, count);
	}

	void Renderer::drawElements(const VertexArray& vao, const u32& mode, const i32& count, const u32& type, const void* indices) {
		vao.bind();
		glDrawElements(mode, count, type, indices);
	}

	void Renderer::drawArraysLines(const VertexArray& vao, const i32& count, const i32& first) {
		this->drawArrays(vao, GL_LINES, first, count);
	}

	void Renderer::drawElementsLines(const VertexArray& vao, const i32& count) {
		this->drawElements(vao, GL_LINES, count, GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::drawArraysTriangles(const VertexArray& vao, const i32& count, const i32& first) {
		this->drawArrays(vao, GL_TRIANGLES, first, count);
	}
	
	void Renderer::drawArraysTriangleFan(const VertexArray& vao, const i32& count, const i32& first) {
		this->drawArrays(vao, GL_TRIANGLE_FAN, first, count);
	}

	void Renderer::drawElementsTriangles(const VertexArray& vao, const i32& count) {
		this->drawElements(vao, GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}
} // namespace bolt
