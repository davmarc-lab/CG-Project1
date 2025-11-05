#include "../include/Factory.hpp"
#include "../include/HermiteFactory.hpp"

#include "../include/ECS/Ett.hpp"
#include "../include/ECS/System.hpp"

#include <glm/glm.hpp>

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

const auto em = EntityManager::instance();

unsigned int factoryBackground(const BasicInfo &info, const glm::vec4 &color) {
	auto id = em->createEntity();
	em->addComponent<Transform>(id);
	systems::transform::updatePosition(id, info.position);
	systems::transform::updateScale(id, info.scale);
	systems::transform::updateRotation(id, info.rotation);
	auto vc = em->addComponent<VertexComponent>(id, squareVertices, getColorVector(color, squareVertices.size()), squareIndices);
	auto bc = em->addComponent<BufferComponent>(id);
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

	auto cc = em->addComponent<RenderComponent>(id);
	cc->setRenderCall([vc, bc]() {
		bc->vao.bind();
		glDrawElements(GL_TRIANGLES, vc->getIndexCoords().size(), GL_UNSIGNED_INT, 0);
	});
	return id;
}

unsigned int factorySquare(const BasicInfo &info, const glm::vec4 &color, const EnemyInfo &stats) {
	auto id = em->createEntity();
	em->addComponent<Transform>(id);
	systems::transform::updatePosition(id, info.position);
	systems::transform::updateScale(id, info.scale);
	systems::transform::updateRotation(id, info.rotation);
	auto vc = em->addComponent<VertexComponent>(id, squareVertices, getColorVector(color, squareVertices.size()), squareIndices);
	auto bc = em->addComponent<BufferComponent>(id);
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

	em->addComponent<Outlined>(id);
	auto cc = em->addComponent<RenderComponent>(id);
	cc->setRenderCall([vc, bc]() {
		bc->vao.bind();
		glDrawElements(GL_TRIANGLES, vc->getIndexCoords().size(), GL_UNSIGNED_INT, 0);
	});
	return id;
}

unsigned int factoryCircle(const BasicInfo &info, const glm::vec4 &color, const unsigned int &numTriangles, const Pair<float> &center, const Pair<float> &radius, const EnemyInfo &stats) {
	auto id = em->createEntity();
	em->addComponent<Transform>(id);
	systems::transform::updatePosition(id, info.position);
	systems::transform::updateScale(id, info.scale);
	systems::transform::updateRotation(id, info.rotation);
	auto vertex = getCircleVertices(center, radius, numTriangles, color, color);
	auto vc = em->addComponent<VertexComponent>(id, vertex.coords, vertex.colors, std::vector<unsigned int>{});
	auto bc = em->addComponent<BufferComponent>(id);
	bc->vao.onAttach();
	bc->vao.bind();

	bc->vbo_g.onAttach();
	bc->vbo_g.setup(vc->getVertexCoords().data(), vc->getVertexCoords().size(), GL_STATIC_DRAW);
	bc->vao.linkAttribFast(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

	bc->vbo_c.onAttach();
	bc->vbo_c.setup(vc->getColorsCoords().data(), vc->getColorsCoords().size(), GL_STATIC_DRAW);
	bc->vao.linkAttribFast(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);

	auto cc = em->addComponent<RenderComponent>(id);
	em->addComponent<Outlined>(id);
	cc->setRenderCall([vc, bc]() {
		bc->vao.bind();
		glDrawArrays(GL_TRIANGLE_FAN, 0, vc->getVertexCoords().size());
	});
	return id;
}

unsigned int factoryProjectile(const BasicInfo &info, const glm::vec4 &color, const ProjInfo &projInfo, const glm::vec3 &direction) {
	auto id = em->createEntity();
	em->addComponent<Transform>(id);
	systems::transform::updatePosition(id, info.position);
	systems::transform::updateScale(id, info.scale);
	systems::transform::updateRotation(id, info.rotation);
	auto vertex = getCircleVertices({0, 0}, {1, 1}, 20, color, color);
	auto vc = em->addComponent<VertexComponent>(id, vertex.coords, vertex.colors, std::vector<unsigned int>{});
	auto bc = em->addComponent<BufferComponent>(id);
	em->addComponent<AABB>(id);
	systems::collision::updateCollider(id);
	bc->vao.onAttach();
	bc->vao.bind();

	bc->vbo_g.onAttach();
	bc->vbo_g.setup(vc->getVertexCoords().data(), vc->getVertexCoords().size(), GL_STATIC_DRAW);
	bc->vao.linkAttribFast(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

	bc->vbo_c.onAttach();
	bc->vbo_c.setup(vc->getColorsCoords().data(), vc->getColorsCoords().size(), GL_STATIC_DRAW);
	bc->vao.linkAttribFast(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);

	// animation
	em->addComponent<ProjectileComponent>(id, info.position, projInfo.range, projInfo.damage, [id, direction]() {
		systems::transform::addPosition(id, direction * PROJ_VEL);
	});

	em->addComponent<Outlined>(id);
	auto cc = em->addComponent<RenderComponent>(id);
	cc->setRenderCall([vc, bc]() {
		bc->vao.bind();
		glDrawArrays(GL_TRIANGLE_FAN, 0, vc->getVertexCoords().size());
	});
	return id;
}

unsigned int factoryHermite(const BasicInfo &info, const std::string &path, const glm::vec4 &color) {
	auto id = em->createEntity();
	em->addComponent<Transform>(id);
	systems::transform::updatePosition(id, info.position);
	systems::transform::updateScale(id, info.scale);
	systems::transform::updateRotation(id, info.rotation);
	// retrieving vertices
	auto curve = readDataFromFile(path.c_str());
	buildHermite(color, color, curve);
	auto vc = em->addComponent<VertexComponent>(id, curve->vertex, curve->colors, std::vector<unsigned int>{});
	auto bc = em->addComponent<BufferComponent>(id);
    em->addComponent<AABB>(id);
	systems::collision::updateCollider(id);
	bc->vao.onAttach();
	bc->vao.bind();

	bc->vbo_g.onAttach();
	bc->vbo_g.setup(vc->getVertexCoords().data(), vc->getVertexCoords().size(), GL_STATIC_DRAW);
	bc->vao.linkAttribFast(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

	bc->vbo_c.onAttach();
	bc->vbo_c.setup(vc->getColorsCoords().data(), vc->getColorsCoords().size(), GL_STATIC_DRAW);
	bc->vao.linkAttribFast(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);

	em->addComponent<Outlined>(id);
	auto cc = em->addComponent<RenderComponent>(id);
	cc->setRenderCall([vc, bc]() {
		bc->vao.bind();
		glDrawArrays(GL_TRIANGLE_FAN, 0, vc->getVertexCoords().size());
	});
	return id;
}

unsigned int factoryEnemy(const BasicInfo &info, const glm::vec4 &color) {
	auto id = em->createEntity();
	em->addComponent<Transform>(id);
	systems::transform::updatePosition(id, info.position);
	systems::transform::updateScale(id, info.scale);
	systems::transform::updateRotation(id, info.rotation);
	// retrieving vertices
	auto curve = readDataFromFile("./resources/hermite/slime.txt");
	buildHermite(color, color, curve);
	auto vc = em->addComponent<VertexComponent>(id, curve->vertex, curve->colors, std::vector<unsigned int>{});
	auto bc = em->addComponent<BufferComponent>(id);
	em->addComponent<EnemyComponent>(id);
	em->addComponent<HealthComponent>(id);
	em->addComponent<AABB>(id);
	systems::collision::updateCollider(id);
	bc->vao.onAttach();
	bc->vao.bind();

	bc->vbo_g.onAttach();
	bc->vbo_g.setup(vc->getVertexCoords().data(), vc->getVertexCoords().size(), GL_STATIC_DRAW);
	bc->vao.linkAttribFast(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

	bc->vbo_c.onAttach();
	bc->vbo_c.setup(vc->getColorsCoords().data(), vc->getColorsCoords().size(), GL_STATIC_DRAW);
	bc->vao.linkAttribFast(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);

	em->addComponent<Outlined>(id);
	auto cc = em->addComponent<RenderComponent>(id);
	cc->setRenderCall([vc, bc]() {
		bc->vao.bind();
		glDrawArrays(GL_TRIANGLE_FAN, 0, vc->getVertexCoords().size());
	});
	return id;
}
