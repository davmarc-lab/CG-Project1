#include "../include/Utils.hpp"

#include "../../Bolt-Core/include/Graphics.hpp"

using namespace bolt;

b8 AABB::isColliding(const AABB &a, const AABB &b) {
	return a.botLeft.x <= b.topRight.x && a.topRight.x >= b.botLeft.x &&
		a.botLeft.y <= b.topRight.y && a.topRight.y >= b.botLeft.y;
}

void initSquare(const u32 &id, Pair<f32> pos, const Pair<f32> &size, const vec4 &color, const Pair<std::string> &shaders) {
	const auto em = EntityManager::instance();
	Shared<Mesh> comp;

	// this give some problem sif the mesh has already Transform component
	if (!em->entityHasComponent<Transform>(id)) {
		const auto t = em->addComponent<Transform>(id);
		t->setPosition({pos.x, pos.y, 0});
		t->setScale({size.x, size.y, 1});
	}

	if (!em->entityHasComponent<Mesh>(id)) {
		// create Mesh
		comp = em->addComponent<Mesh>(id);

		comp->vertices = factory::mesh::squareGeometry;
		comp->colorComponent.colors = factory::mesh::getColorVector(comp->vertices.size(), color);
	} else {
		comp = em->getEntityComponent<Mesh>(id);
	}

	// instance mesh on GPU
	comp->vao.onAttach();
	comp->vbo_g.onAttach();
	comp->colorComponent.vbo_c.onAttach();

	comp->vao.bind();
	comp->vbo_g.setup(comp->vertices, 0);
	comp->vao.linkAttribFast(0, 3, 0, false, 0, 0);

	comp->colorComponent.vbo_c.setup(comp->colorComponent.colors, 0);
	comp->vao.linkAttribFast(1, 4, 0, false, 0, 0);

	// problem if the Mesh component already exists
	auto shaderComp = em->addComponent<ShaderComponent>(id);
	shaderComp->shader = CreateUnique<ShaderProgram>(shaders.x, shaders.y);
	shaderComp->shader->createShaderProgram();

	auto vao = comp->vao;
	auto num = comp->vertices.size();

	comp->render.setCall([vao, num]() {
		RenderApi::instance()->getRenderer()->drawArraysTriangles(vao, num, 0);
	});
}

void initCircle(const u32 &id, bolt::Pair<f32> pos, const bolt::Pair<f32> &size, const bolt::vec4 &mid, const bolt::vec4 &out, const bolt::Pair<std::string> &shaders, const CircleSettings &settings) {
	const auto em = EntityManager::instance();
	Shared<Mesh> comp;

	// this give some problem sif the mesh has already Transform component
	if (!em->entityHasComponent<Transform>(id)) {
		const auto t = em->addComponent<Transform>(id);
		t->setPosition({pos.x, pos.y, 0});
		t->setScale({size.x, size.y, 1});
	}

	if (!em->entityHasComponent<Mesh>(id)) {
		// create Mesh
		comp = em->addComponent<Mesh>(id);
		auto vert = factory::mesh::getCircleVertices(settings.center, settings.radius, settings.divisions, mid, out);
		comp->vertices = vert.vertices;
		comp->colorComponent.colors = vert.colors;
	} else {
		comp = em->getEntityComponent<Mesh>(id);
	}

	// instance mesh on GPU
	comp->vao.onAttach();
	comp->vbo_g.onAttach();
	comp->colorComponent.vbo_c.onAttach();

	comp->vao.bind();
	comp->vbo_g.setup(comp->vertices, 0);
	comp->vao.linkAttribFast(0, 3, 0, false, 0, 0);

	comp->colorComponent.vbo_c.setup(comp->colorComponent.colors, 0);
	comp->vao.linkAttribFast(1, 4, 0, false, 0, 0);

	// problem if the Mesh component already exists
	auto shaderComp = em->addComponent<ShaderComponent>(id);
	shaderComp->shader = CreateUnique<ShaderProgram>(shaders.x, shaders.y);
	shaderComp->shader->createShaderProgram();

	auto vao = comp->vao;
	auto aaa = comp->vertices.size();

	comp->render.setCall([vao, aaa]() {
		RenderApi::instance()->getRenderer()->drawArraysTriangleFan(vao, aaa, 0);
	});
}

vec3 prePos;

void MultiMesh::addMesh(const u32 &id) {
	if (this->m_anchor == -1)
		this->m_anchor = static_cast<i32>(id);
	this->m_meshes.push_back(id);
}

void MultiMesh::setAnchor(const u32 &id) {
	this->m_anchor = static_cast<i32>(id);
}

void MultiMesh::moveAnchor(const bolt::vec3 &position, const bolt::vec3 &scale, const bolt::vec3 &rotation, const f32 &rotVal) const {
	auto val = rotVal;
	auto rot = rotation;

	auto translate = glm::translate(base, position);
	auto scl = glm::scale(base, scale);

	if (rotation == vec3(0)) {
		rot = {1, 1, 1};
		val = 0;
	}
	const auto rotate = glm::rotate(base, radians(val), rot);

	const auto anchor = EntityManager::instance()->getEntityComponent<Transform>(this->m_anchor);
	prePos = anchor->getModelMatrix()[3];

	anchor->setModelMatrix(anchor->getModelMatrix() * scl * rotate * translate);
}

void MultiMesh::transform(const bolt::vec3 &position, const bolt::vec3 &scale, const bolt::vec3 &rotation, const f32 &rotVal) const {
	const auto em = EntityManager::instance();
	const auto anchor = em->getEntityComponent<Transform>(this->m_anchor);
	this->moveAnchor(position, scale, rotation, rotVal);

	auto val = rotVal;
	auto rot = rotation;

	vec3 anchorPos = anchor->getModelMatrix()[3];
	auto final = anchorPos - prePos;
	for (auto id : this->m_meshes) {
		if (this->m_anchor == id)
			continue;

		auto translate = bolt::translate(base, final);
		auto scl = bolt::scale(base, scale);
		if (rotation == vec3(0)) {
			rot = {1, 1, 1};
			val = 0;
		}
		auto rotate = glm::rotate(base, radians(val), rot);

		auto mesh = em->getEntityComponent<Transform>(id);
		auto result = mesh->getModelMatrix() * scl * rotate * translate;
		result[3] = vec4(final + vec3(mesh->getModelMatrix()[3]), 1);
		mesh->setModelMatrix(result);
	}
}
