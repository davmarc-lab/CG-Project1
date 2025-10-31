#include "../../include/Core/Renderer.hpp"
#include <GLFW/glfw3.h>

#include "../../include/Core/Event.hpp"

#include "../../include/Graphic.hpp"

#include "../../include/Core/MeshVertices.hpp"

glm::mat4 calcModelMatrix(const glm::vec3 &pos, const glm::vec3 &dim, const glm::vec3 &rot) {
	auto base = glm::mat4(1);
	base = glm::translate(base, pos);
	base = glm::scale(base, dim);
	return base;
}

namespace ogl {
	Unique<ShaderProgram> rendShader;
	Unique<ShaderProgram> instancedShader;

	void Renderer::clear() {
		this->m_cube.clear();
		this->m_sphere.clear();
		this->m_pyramid.clear();
		this->m_thorus.clear();
		rendShader->clear();
		instancedShader->clear();

		this->m_init = false;
	}

	void Renderer::init() {
		rendShader = CreateUnique<ShaderProgram>("vertexShader.glsl", "fragmentShader.glsl");
		instancedShader = CreateUnique<ShaderProgram>("instancedVertShader.glsl", "instancedFragShader.glsl");
		rendShader->createShaderProgram();
		instancedShader->createShaderProgram();

		// cube
		this->m_cube.vao.onAttach();
		this->m_cube.vao.bind();

		this->m_cube.vertex = cubeGeometry;
		this->m_cube.colors = getColorVector({1, 0, 0, 1}, cubeGeometry.size());
		this->m_cube.index = cubeIndices;
		this->m_cube.texCoords = cubeTexCoord;
		this->m_cube.normals = cubeNormals;

		this->m_cube.vbog.onAttach();
		this->m_cube.vbog.setup(this->m_cube.vertex.data(), this->m_cube.vertex.size(), GL_STATIC_DRAW);
		this->m_cube.vao.linkAttribFast(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
		this->m_cube.vboc.onAttach();
		this->m_cube.vboc.setup(this->m_cube.colors.data(), this->m_cube.colors.size(), GL_STATIC_DRAW);
		this->m_cube.vao.linkAttribFast(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
		this->m_cube.vbon.onAttach();
		this->m_cube.vbon.setup(this->m_cube.normals.data(), this->m_cube.normals.size(), GL_STATIC_DRAW);
		this->m_cube.vao.linkAttribFast(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
		this->m_cube.vbot.onAttach();
		this->m_cube.vbot.setup(this->m_cube.texCoords.data(), this->m_cube.texCoords.size(), GL_STATIC_DRAW);
		this->m_cube.vao.linkAttribFast(3, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

		this->m_cube.ebo.onAttach();
		this->m_cube.ebo.setup(this->m_cube.index.data(), this->m_cube.index.size(), GL_STATIC_DRAW);
		this->m_cube.vao.unbind();

		// pyramid
		this->m_pyramid.vao.onAttach();
		this->m_pyramid.vao.bind();

		this->m_pyramid.vertex = pyramidGeometry;
		this->m_pyramid.colors = getColorVector({1, 0, 0, 1}, pyramidGeometry.size());
		this->m_pyramid.index = pyramidIndices;
		this->m_pyramid.texCoords = pyramidTexCoords;
		this->m_pyramid.normals = pyramidNormals;

		this->m_pyramid.vbog.onAttach();
		this->m_pyramid.vbog.setup(this->m_pyramid.vertex.data(), this->m_pyramid.vertex.size(), GL_STATIC_DRAW);
		this->m_pyramid.vao.linkAttribFast(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
		this->m_pyramid.vboc.onAttach();
		this->m_pyramid.vboc.setup(this->m_pyramid.colors.data(), this->m_pyramid.colors.size(), GL_STATIC_DRAW);
		this->m_pyramid.vao.linkAttribFast(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
		this->m_pyramid.vbon.onAttach();
		this->m_pyramid.vbon.setup(this->m_pyramid.normals.data(), this->m_pyramid.normals.size(), GL_STATIC_DRAW);
		this->m_pyramid.vao.linkAttribFast(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
		this->m_pyramid.vbot.onAttach();
		this->m_pyramid.vbot.setup(this->m_pyramid.texCoords.data(), this->m_pyramid.texCoords.size(), GL_STATIC_DRAW);
		this->m_pyramid.vao.linkAttribFast(3, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

		this->m_pyramid.ebo.onAttach();
		this->m_pyramid.ebo.setup(this->m_pyramid.index.data(), this->m_pyramid.index.size(), GL_STATIC_DRAW);
		this->m_pyramid.vao.unbind();

		// sphere
		this->m_sphere.vao.onAttach();
		this->m_sphere.vao.bind();

		auto coords = getSphereVertices();
		this->m_sphere.vertex = coords.vertex;
		this->m_sphere.colors = getColorVector({1, 0, 0, 1}, coords.vertex.size());
		this->m_sphere.index = coords.indices;
		this->m_sphere.texCoords = coords.texCoords;
		this->m_sphere.normals = coords.normals;

		this->m_sphere.vbog.onAttach();
		this->m_sphere.vbog.setup(this->m_sphere.vertex.data(), this->m_sphere.vertex.size(), GL_STATIC_DRAW);
		this->m_sphere.vao.linkAttribFast(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
		this->m_sphere.vboc.onAttach();
		this->m_sphere.vboc.setup(this->m_sphere.colors.data(), this->m_sphere.colors.size(), GL_STATIC_DRAW);
		this->m_sphere.vao.linkAttribFast(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
		this->m_sphere.vbon.onAttach();
		this->m_sphere.vbon.setup(this->m_sphere.normals.data(), this->m_sphere.normals.size(), GL_STATIC_DRAW);
		this->m_sphere.vao.linkAttribFast(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
		this->m_sphere.vbot.onAttach();
		this->m_sphere.vbot.setup(this->m_sphere.texCoords.data(), this->m_sphere.texCoords.size(), GL_STATIC_DRAW);
		this->m_sphere.vao.linkAttribFast(3, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

		// Instanced Buffers
		this->m_sphere.vboco.onAttach();

		this->m_sphere.vbomo.onAttach();

		this->m_sphere.ebo.onAttach();
		this->m_sphere.ebo.setup(this->m_sphere.index.data(), this->m_sphere.index.size(), GL_STATIC_DRAW);
		this->m_sphere.vao.unbind();

		// thorus
		this->m_thorus.vao.onAttach();
		this->m_thorus.vao.bind();

		coords = getThorusVertices();
		this->m_thorus.vertex = coords.vertex;
		this->m_thorus.colors = getColorVector({1, 0, 0, 1}, coords.vertex.size());
		this->m_thorus.index = coords.indices;
		this->m_thorus.texCoords = coords.texCoords;
		this->m_thorus.normals = coords.normals;

		this->m_thorus.vbog.onAttach();
		this->m_thorus.vbog.setup(this->m_thorus.vertex.data(), this->m_thorus.vertex.size(), GL_STATIC_DRAW);
		this->m_thorus.vao.linkAttribFast(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
		this->m_thorus.vboc.onAttach();
		this->m_thorus.vboc.setup(this->m_thorus.colors.data(), this->m_thorus.colors.size(), GL_STATIC_DRAW);
		this->m_thorus.vao.linkAttribFast(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
		this->m_thorus.vbon.onAttach();
		this->m_thorus.vbon.setup(this->m_thorus.normals.data(), this->m_thorus.normals.size(), GL_STATIC_DRAW);
		this->m_thorus.vao.linkAttribFast(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
		this->m_thorus.vbot.onAttach();
		this->m_thorus.vbot.setup(this->m_thorus.texCoords.data(), this->m_thorus.texCoords.size(), GL_STATIC_DRAW);
		this->m_thorus.vao.linkAttribFast(3, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

		this->m_thorus.ebo.onAttach();
		this->m_thorus.ebo.setup(this->m_thorus.index.data(), this->m_thorus.index.size(), GL_STATIC_DRAW);
		this->m_thorus.vao.unbind();

		this->m_init = true;
	}

	void Renderer::appendSphere(const unsigned int &id, const glm::mat4 &model, const glm::vec4 &color) {
		ASSERT(this->m_init);
		this->m_stats.numSpheres++;
		this->m_sphere.modelOffset.push_back(model);
		this->m_sphere.colorOffset.push_back(color);
		this->m_sphere.vao.bind();
		this->m_sphere.vboco.setup(this->m_sphere.colorOffset.data(), this->m_sphere.colorOffset.size(), GL_STATIC_DRAW);
		this->m_sphere.vao.linkAttribFast(4, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
		glVertexAttribDivisor(4, 1);

		this->m_sphere.vbomo.setup(this->m_sphere.modelOffset.data(), this->m_sphere.modelOffset.size(), GL_STATIC_DRAW);
		this->m_sphere.vao.linkAttribFast(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void *)0);
		this->m_sphere.vao.linkAttribFast(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void *)(sizeof(glm::vec4)));
		this->m_sphere.vao.linkAttribFast(7, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void *)(2 * sizeof(glm::vec4)));
		this->m_sphere.vao.linkAttribFast(8, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void *)(3 * sizeof(glm::vec4)));
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);
		glVertexAttribDivisor(8, 1);
	}

	void Renderer::appendCube(const unsigned int &id, const glm::mat4 &model, const glm::vec4 &color) {
		ASSERT(this->m_init);
		this->m_stats.numCubes++;
		this->m_cube.modelOffset.push_back(model);
		this->m_cube.colorOffset.push_back(color);
		this->m_cube.vao.bind();
		this->m_cube.vboco.setup(this->m_cube.colorOffset.data(), this->m_cube.colorOffset.size(), GL_STATIC_DRAW);
		this->m_cube.vao.linkAttribFast(4, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
		glVertexAttribDivisor(4, 1);

		this->m_cube.vbomo.setup(this->m_cube.modelOffset.data(), this->m_cube.modelOffset.size(), GL_STATIC_DRAW);
		this->m_cube.vao.linkAttribFast(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void *)0);
		this->m_cube.vao.linkAttribFast(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void *)(sizeof(glm::vec4)));
		this->m_cube.vao.linkAttribFast(7, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void *)(2 * sizeof(glm::vec4)));
		this->m_cube.vao.linkAttribFast(8, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void *)(3 * sizeof(glm::vec4)));
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);
		glVertexAttribDivisor(8, 1);
	}

	void Renderer::prepareBuffers(const std::vector<glm::mat4> &models, const std::vector<glm::vec4> &colors) {
		ASSERT(this->m_init);
		if (this->m_stats.numSpheres) {
			this->m_sphere.vao.bind();
			this->m_sphere.vbomo.setup(models.data(), models.size(), GL_STATIC_DRAW);
			this->m_sphere.vboco.setup(colors.data(), colors.size(), GL_STATIC_DRAW);
		}
	}

	void Renderer::drawArrays(const unsigned int &vao, const unsigned int &mode, const int &first, const size_t &size) {
		glBindVertexArray(vao);
		glDrawArrays(mode, first, size);
		this->m_stats.drawCalls++;
	}

	void Renderer::drawElements(const unsigned int &vao, const unsigned int &mode, const size_t &size, const unsigned int &type, const void *indices) {
		glBindVertexArray(vao);
		glDrawElements(mode, size, type, indices);
		this->m_stats.drawCalls++;
	}

	void Renderer::drawAllInstanced() {
		instancedShader->use();
		if (this->m_stats.numSpheres) {
			this->m_sphere.vao.bind();
			glDrawElementsInstanced(GL_TRIANGLES, this->m_sphere.index.size(), GL_UNSIGNED_INT, 0, this->m_stats.numSpheres);
		}
	}

} // namespace ogl
