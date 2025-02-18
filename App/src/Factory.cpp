#include "../include/Factory.hpp"
#include <glm/ext/scalar_constants.hpp>
#include <glm/ext/vector_float4.hpp>
#include "../include/HermiteFactory.hpp"

#include "../include/ECS/Ett.hpp"
#include "../include/ECS/System.hpp"

const std::vector<glm::vec3> squareVertices{
	{-1, -1, 0},
	{1, -1, 0},
	{1, 1, 0},
	{-1, 1, 0},
};
const std::vector<unsigned int> squareIndices{0, 1, 2, 0, 2, 3};

const std::vector<glm::vec3> triangleVertices{
	{-1, -1, 0},
	{1, -1, 0},
	{0, 1, 0},
};

EntityVertex getCircleVertices(const Pair<float> &center = {0, 0}, const Pair<float> &radius = {1, 1}, const unsigned int numTriangles = 20, const glm::vec4 &centerColor = {1, 0, 0, 1}, const glm::vec4 &outerColor = {1, 0, 0, 1}) {
	EntityVertex res{};

	float stepA = (2 * glm::pi<float>()) / static_cast<float>(numTriangles);
	float t, xx, yy;

	res.coords.emplace_back(center.x, center.y, 0.0f);
	res.colors.push_back(centerColor);

	for (int i = 0; i <= numTriangles; i++) {
		t = (float)i * stepA;
		xx = center.x + radius.x * cos(t);
		yy = center.y + radius.y * sin(t);

		res.coords.emplace_back(xx, yy, 0.0f);
		res.colors.push_back(outerColor);
	}

	return res;
}

std::vector<glm::vec4> getColorVector(const glm::vec4 &color, const size_t &size) {
	auto res = std::vector<glm::vec4>{};
	for (size_t i = 0; i < size; i++) {
		res.push_back(color);
	}
	return res;
}

const auto em = EntityManager::instance();

Shared<Entity> createSquare(const glm::vec3 &pos, const glm::vec3 &scale, const glm::vec3 &rot, const glm::vec4 &color) {
	return CreateShared<Entity>(EntityVertex{squareVertices, getColorVector(color, squareVertices.size()), squareIndices},
								EntityModel{pos, scale, rot});
}

Shared<Entity> createCircle(const glm::vec3 &pos, const glm::vec3 &scale, const glm::vec3 &rot, const Pair<float> &center, const Pair<float> &radius,
							const unsigned int &numTriangles, const glm::vec4 &color) {
	EntityModel model{pos, scale, rot};
	EntityVertex vertex{};

	float stepA = (2 * glm::pi<float>()) / static_cast<float>(numTriangles);
	float t, xx, yy;

	vertex.coords.emplace_back(center.x, center.y, 0.0f);
	vertex.colors.push_back(color);

	for (int i = 0; i <= numTriangles; i++) {
		t = (float)i * stepA;
		xx = center.x + radius.x * cos(t);
		yy = center.y + radius.y * sin(t);

		vertex.coords.emplace_back(xx, yy, 0.0f);
		vertex.colors.push_back(color);
	}
	return CreateShared<Entity>(vertex, model);
}

Shared<Entity> createTriangle(const glm::vec3 &pos, const glm::vec3 &scale, const glm::vec3 &rot, const glm::vec4 &color) {
	return CreateShared<Entity>(EntityVertex{triangleVertices, getColorVector(color, triangleVertices.size()), std::vector<unsigned int>{}},
								EntityModel{pos, scale, rot});
}

Shared<Entity> createProjectile(const glm::vec3 &pos, const glm::vec3 &scale, const glm::vec3 &rot, const glm::vec4 &color) {
	return createCircle(pos, scale, rot, {0, 0}, {1, 1}, 30, color);
}

Shared<MultiShape> createEnemy(const ogl::ShaderProgram &shader, const EnemyType &type, const BasicInfo &info) {
	Shared<MultiShape> e;
	switch (type) {
		case EMENY_SLIME: {
			auto herm = readDataFromFile("./resources/hermite/slime.txt");
			buildHermite({1, 0, 0, 1}, {1, 0, 0, 1}, herm);
			Shared<Enemy> tmp = CreateShared<Enemy>(ENEMY_SLIME_INFO, EntityVertex{herm->vertex, herm->colors, {}},
													EntityModel{info.position, info.scale, info.rotation});
			tmp->init();
			tmp->setRenderCall([tmp, &shader]() {
				shader.setMat4("model", tmp->getModelMatrix());
				tmp->bindVAO();
				glDrawArrays(GL_TRIANGLE_FAN, 0, tmp->getCoordsVector().size());
			});
			e = CreateShared<MultiShape>(shader, tmp);
			auto leye = createCircle(info.position + SLIME_LEYE_OFFSET, SLIME_EYE_SCALE, {}, {0, 0}, {1, 1}, 30, {0, 0, 0, 1});
			leye->init();
			leye->setRenderCall([leye, &shader]() {
				shader.setMat4("model", leye->getModelMatrix());
				leye->bindVAO();
				glDrawArrays(GL_TRIANGLE_FAN, 0, leye->getCoordsVector().size());
			});
			auto reye = createCircle(info.position + SLIME_REYE_OFFSET, SLIME_EYE_SCALE, {}, {0, 0}, {1, 1}, 30, {0, 0, 0, 1});
			reye->init();
			reye->setRenderCall([reye, &shader]() {
				shader.setMat4("model", reye->getModelMatrix());
				reye->bindVAO();
				glDrawArrays(GL_TRIANGLE_FAN, 0, reye->getCoordsVector().size());
			});
			e->addEntity(leye);
			e->addEntity(reye);
			break;
		}
		case ENEMY_FLY: {
			break;
		}
		default:
			return nullptr;
	}
	return e;
}

unsigned int factorySquare(const BasicInfo &info, const glm::vec4 &color, const EnemyInfo &stats) {
	auto id = em->createEntity();
	auto tc = em->addComponent<Transform>(id);
	systems::transform::updatePosition(id, info.position);
	systems::transform::updateScale(id, info.scale);
	systems::transform::updateRotation(id, info.rotation);
	auto vc = em->addComponent<VertexComponent>(id, squareVertices, getColorVector(color, squareVertices.size()), squareIndices);
	auto bc = em->addComponent<BufferComponent>(id);
	auto coll = em->addComponent<AABB>(id);
	bc->vao.onAttach();
	bc->vao.bind();

	bc->vbo_g.onAttach();
	bc->vbo_g.setup(vc->getVertexCoords().data(), vc->getVertexCoords().size(), GL_STATIC_DRAW);
	bc->vao.linkAttribFast(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

	bc->vbo_c.onAttach();
	bc->vbo_c.setup(vc->getColorsCoords().data(), vc->getColorsCoords().size(), GL_STATIC_DRAW);
	bc->vao.linkAttribFast(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);

	bc->ebo.onAttach();
	bc->ebo.setup(vc->getIndexCoords().data(), vc->getIndexCoords().size(), GL_STATIC_DRAW);

	auto ic = em->addComponent<InputComponent>(id);
	auto cc = em->addComponent<RenderComponent>(id);
	cc->setRenderCall([vc, bc]() {
		bc->vao.bind();
		glDrawElements(GL_TRIANGLES, vc->getIndexCoords().size(), GL_UNSIGNED_INT, 0);
	});
	return id;
}

unsigned int factoryProjectile(const BasicInfo &info, const glm::vec4 &color, const ProjInfo &projInfo) {
	auto id = em->createEntity();
	auto tc = em->addComponent<Transform>(id);
	systems::transform::updatePosition(id, info.position);
	systems::transform::updateScale(id, info.scale);
	systems::transform::updateRotation(id, info.rotation);
	auto vertex = getCircleVertices({0, 0}, {1, 1}, 20, color, color);
	auto vc = em->addComponent<VertexComponent>(id, vertex.coords, vertex.colors, std::vector<unsigned int>{});
	auto bc = em->addComponent<BufferComponent>(id);
	auto coll = em->addComponent<AABB>(id);
	bc->vao.onAttach();
	bc->vao.bind();

	bc->vbo_g.onAttach();
	bc->vbo_g.setup(vc->getVertexCoords().data(), vc->getVertexCoords().size(), GL_STATIC_DRAW);
	bc->vao.linkAttribFast(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

	bc->vbo_c.onAttach();
	bc->vbo_c.setup(vc->getColorsCoords().data(), vc->getColorsCoords().size(), GL_STATIC_DRAW);
	bc->vao.linkAttribFast(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);

	auto ic = em->addComponent<InputComponent>(id);
	auto cc = em->addComponent<RenderComponent>(id);
	cc->setRenderCall([vc, bc]() {
		bc->vao.bind();
		glDrawArrays(GL_TRIANGLE_FAN, 0, vc->getVertexCoords().size());
	});
	return id;
}
