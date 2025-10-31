#pragma once

#include "../Buffer/ElementBuffer.hpp"
#include "../Buffer/VertexArray.hpp"
#include "../Buffer/VertexBuffer.hpp"

#include "../Shader/Shader.hpp"

#include "Utils.hpp"

#include <cstddef>
#include <vector>

namespace ogl {
	struct Primitive {
		VertexArray vao{};
		VertexBuffer vbog{};
		std::vector<glm::vec3> vertex{};
		VertexBuffer vboc{};
		std::vector<glm::vec4> colors{};
		ElementBuffer ebo{};
		std::vector<unsigned int> index{};
		VertexBuffer vbot{};
		std::vector<glm::vec2> texCoords{};
		VertexBuffer vbon{};
		std::vector<glm::vec3> normals{};

		std::vector<unsigned int> ids{};
		VertexBuffer vbomo{};
		std::vector<glm::mat4> modelOffset{};
		VertexBuffer vboco{};
		std::vector<glm::vec4> colorOffset{};

		inline void clear() {
			this->vbog.onDetach();
			this->vboc.onDetach();
			this->vbot.onDetach();
			this->vbon.onDetach();
			this->vbomo.onDetach();
			this->vboco.onDetach();
			this->ebo.onDetach();
			this->vao.onDetach();
		}
	};

	struct RendererStats {
		int drawCalls = 0;
		unsigned int numCubes = 0;
		unsigned int numPyramids = 0;
		unsigned int numSpheres = 0;
		unsigned int numThorus = 0;
	};

	enum RenderPrimitiveType {
		PRIMITIVE_CUBE,
		PRIMITIVE_SPHERE,
		PRIMITIVE_PYRAMID,
		PRIMITIVE_THORUS,
	};

	class Renderer {
	public:
		void init();
		void clear();

		void appendSphere(const unsigned int &id, const glm::mat4 &model, const glm::vec4 &color);
		void appendCube(const unsigned int &id, const glm::mat4 &model, const glm::vec4 &color);
		void appendPyramid(const unsigned int &id, const glm::mat4 &model, const glm::vec4 &color);
		void appendThorus(const unsigned int &id, const glm::mat4 &model, const glm::vec4 &color);

		void drawArrays(const unsigned int &vao, const unsigned int &mode, const int &first, const size_t &size);

		void drawElements(const unsigned int &vao, const unsigned int &mode, const size_t &size, const unsigned int &type, const void *indices = 0);

		void drawAllInstanced();

		void prepareBuffers(const std::vector<glm::mat4> &models, const std::vector<glm::vec4> &colors);

		RendererStats getRendererStats() const { return this->m_stats; }

		void resetDrawStats() {
			this->m_stats.numCubes = 0;
			this->m_stats.numPyramids = 0;
			this->m_stats.numSpheres = 0;
			this->m_stats.numThorus = 0;
		}

		Renderer(Renderer &other) = delete;

		void operator=(const Renderer &other) = delete;

		inline static Shared<Renderer> instance() {
			if (s_pointer == nullptr) {
				Shared<Renderer> copy(new Renderer());
				copy.swap(s_pointer);
			}
			return s_pointer;
		}

		~Renderer() = default;

	private:
		Renderer() = default;

		inline static Shared<Renderer> s_pointer = nullptr;

		RendererStats m_stats{};

		Primitive m_cube{};
		Primitive m_pyramid{};
		Primitive m_sphere{};
		Primitive m_thorus{};
		bool m_init = false;
	};

} // namespace ogl
