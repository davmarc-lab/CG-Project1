#pragma once

#include "../../Bolt-Core/include/Engine.hpp"

struct CircleSettings {
	bolt::Pair<f32> radius{1, 1};
	bolt::Pair<f32> center{0, 0};
	u32 divisions = 50;
};

struct DoubleColor {
    bolt::vec4 out{0, 0, 0, 1};
	bolt::vec4 mid{1, 1, 1, 1};
};

struct AABB {
	bolt::vec3 botLeft;
	bolt::vec3 topRight;

	static b8 isColliding(const AABB& a, const AABB& b);
};

// Non ne vado molto fiero, usa le matrici di modellazione per la posizione....
class MultiMesh {
public:
	void addMesh(const u32 &id);

	void setAnchor(const u32 &id);

	void moveAnchor(const bolt::vec3 &position, const bolt::vec3 &scale, const bolt::vec3 &rotation, const f32 &rotVal)const ;

	void transform(const bolt::vec3 &position, const bolt::vec3 &scale, const bolt::vec3 &rotation, const f32 &rotVal) const;

    AABB getBoundingBox(const u32& id) const;

	MultiMesh() = default;

	MultiMesh(const std::vector<u32> &meshes, const i32 &anchorIndex = -1) :
		m_meshes(meshes) {
		this->m_anchor = anchorIndex == -1 ? static_cast<i32>(this->m_meshes[0]) : anchorIndex;
	}

	~MultiMesh() = default;

private:
	i32 m_anchor = -1;
	std::vector<u32> m_meshes{};
};

void initSquare(const u32 &id, bolt::Pair<f32> pos, const bolt::Pair<f32> &size, const bolt::vec4 &color, const bolt::Pair<std::string> &shaders);

void initCircle(const u32 &id, bolt::Pair<f32> pos, const bolt::Pair<f32> &size, const bolt::vec4 &mid, const bolt::vec4 &out, const bolt::Pair<std::string> &shaders, const CircleSettings &settings);
