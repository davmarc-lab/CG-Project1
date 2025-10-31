#pragma once

#include <glm/ext/scalar_constants.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "Utils.hpp"

const float PI = glm::pi<float>();

/**
 * @brief Utility struct to temporally store mesh vectors.
 */
struct MeshInfo {
	/// mesh normalized position coords vertices
	std::vector<glm::vec3> vertex{};
	/// mesh normal coords vertices
	std::vector<glm::vec3> normals{};
	/// mesh texture coords vertices
	std::vector<glm::vec2> texCoords{};
	/// mesh vertex coords indices
	std::vector<unsigned int> indices{};
};

inline std::vector<glm::vec3> skyboxGeometry{
	// vertex pos
	{-1.0f, -1.0f, -1.0f},
	{1.0f, 1.0f, -1.0f},
	{1.0f, -1.0f, -1.0f},
	{1.0f, 1.0f, -1.0f},
	{-1.0f, -1.0f, -1.0f},
	{-1.0f, 1.0f, -1.0f},

	{-1.0f, -1.0f, 1.0f},
	{1.0f, -1.0f, 1.0f},
	{1.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, 1.0f},
	{-1.0f, 1.0f, 1.0f},
	{-1.0f, -1.0f, 1.0f},

	{-1.0f, 1.0f, 1.0f},
	{-1.0f, 1.0f, -1.0f},
	{-1.0f, -1.0f, -1.0f},
	{-1.0f, -1.0f, -1.0f},
	{-1.0f, -1.0f, 1.0f},
	{-1.0f, 1.0f, 1.0f},

	{1.0f, 1.0f, 1.0f},
	{1.0f, -1.0f, -1.0f},
	{1.0f, 1.0f, -1.0f},
	{1.0f, -1.0f, -1.0f},
	{1.0f, 1.0f, 1.0f},
	{1.0f, -1.0f, 1.0f},

	{-1.0f, -1.0f, -1.0f},
	{1.0f, -1.0f, -1.0f},
	{1.0f, -1.0f, 1.0f},
	{1.0f, -1.0f, 1.0f},
	{-1.0f, -1.0f, 1.0f},
	{-1.0f, -1.0f, -1.0f},

	{-1.0f, 1.0f, -1.0f},
	{1.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, -1.0f},
	{1.0f, 1.0f, 1.0f},
	{-1.0f, 1.0f, -1.0f},
	{-1.0f, 1.0f, 1.0f}};

// cube
inline std::vector<glm::vec3> cubeGeometry{
	{-1, -1, -1},
	{-1, 1, -1},
	{1, -1, -1},
	{1, 1, -1},

	{-1, -1, 1},
	{-1, 1, 1},
	{1, -1, 1},
	{1, 1, 1},

	{-1, -1, 1},
	{-1, 1, 1},
	{-1, -1, -1},
	{-1, 1, -1},

	{1, -1, 1},
	{1, 1, 1},
	{1, -1, -1},
	{1, 1, -1},

	{-1, 1, -1},
	{-1, 1, 1},
	{1, 1, -1},
	{1, 1, 1},

	{-1, -1, -1},
	{-1, -1, 1},
	{1, -1, -1},
	{1, -1, 1},
};

inline std::vector<unsigned int> cubeIndices{
	0, 3, 2, 1, 3, 0,
	6, 7, 4, 4, 7, 5,
	8, 11, 10, 9, 11, 8,
	14, 15, 12, 12, 15, 13,
	16, 19, 18, 17, 19, 16,
	22, 23, 20, 20, 23, 21};
inline std::vector<glm::vec2> cubeTexCoord{
	// texCoord
	{0.0f, 0.0f},
	{1.0f, 0.0f},
	{1.0f, 1.0f},
	{0.0f, 1.0f},

	{0.0f, 0.0f},
	{1.0f, 0.0f},
	{1.0f, 1.0f},
	{0.0f, 1.0f},

	{0.0f, 0.0f},
	{1.0f, 0.0f},
	{1.0f, 1.0f},
	{0.0f, 1.0f},

	{0.0f, 0.0f},
	{1.0f, 0.0f},
	{1.0f, 1.0f},
	{0.0f, 1.0f},

	{0.0f, 0.0f},
	{1.0f, 0.0f},
	{1.0f, 1.0f},
	{0.0f, 1.0f},

	{0.0f, 0.0f},
	{1.0f, 0.0f},
	{1.0f, 1.0f},
	{0.0f, 1.0f},
};

inline std::vector<glm::vec3> cubeNormals{
	{0, 0, -1},
	{0, 0, -1},
	{0, 0, -1},
	{0, 0, -1},
	{0, 0, 1},
	{0, 0, 1},
	{0, 0, 1},
	{0, 0, 1},
	{-1, 0, 0},
	{-1, 0, 0},
	{-1, 0, 0},
	{-1, 0, 0},
	{1, 0, 0},
	{1, 0, 0},
	{1, 0, 0},
	{1, 0, 0},
	{0, 1, 0},
	{0, 1, 0},
	{0, 1, 0},
	{0, 1, 0},
	{0, -1, 0},
	{0, -1, 0},
	{0, -1, 0},
	{0, -1, 0},
};

inline std::vector<glm::vec3> pyramidGeometry{
	// base
	{-1, -1, -1},
	{1, -1, -1},
	{1, -1, 1},
	{-1, -1, 1},

	// back
	{1, -1, 1},
	{0, 1, 0},
	{-1, -1, 1},

	// front
	{-1, -1, -1},
	{0, 1, 0},
	{1, -1, -1},

	// right
	{1, -1, -1},
	{0, 1, 0},
	{1, -1, 1},

	// left
	{-1, -1, 1},
	{0, 1, 0},
	{-1, -1, -1},
};

inline std::vector<unsigned int> pyramidIndices{
	0, 1, 2, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

inline std::vector<glm::vec3> pyramidNormals{
	{0, -1, 0},
	{0, -1, 0},
	{0, -1, 0},
	{0, -1, 0},
	{0, 0, 1},
	{0, 0, 1},
	{0, 0, 1},
	{0, 0, -1},
	{0, 0, -1},
	{0, 0, -1},
	{1, 0, 0},
	{1, 0, 0},
	{1, 0, 0},
	{-1, 0, 0},
	{-1, 0, 0},
	{-1, 0, 0},
	{0, 0, 0},
};

inline std::vector<glm::vec2> pyramidTexCoords{
	{0.f, 0.f},
	{1.f, 0.f},
	{1.f, 1.f},
	{0.f, 1.f},
	{0.f, 0.f},
	{0.5f, 1.f},
	{1.f, 0.f},
	{0.f, 0.f},
	{0.5f, 1.f},
	{1.f, 0.f},
	{0.f, 0.f},
	{0.5f, 1.f},
	{1.f, 0.f},
	{0.f, 0.f},
	{0.5f, 1.f},
	{1.f, 0.f},
};
/**
 * @brief Calculates all sphere vertices with the given parameters
 *
 * @param center center point coords in local space
 * @param radius radius lenght in local space
 * @param stacks number of stacks
 * @param slices number of slices
 *
 * @return sphere points coords
 */
inline MeshInfo getSphereVertices(const glm::vec3 &center = {0, 0, 0}, const glm::vec3 &radius = {1, 1, 1}, const unsigned int &stacks = 30, const unsigned int &slices = 30) {
	MeshInfo coords{};
	float s, t;

	for (auto i = 0; i <= stacks; ++i) {

		float V = i / (float)stacks;
		float phi = V * PI;

		for (int j = 0; j <= slices; ++j) {

			float U = j / (float)slices;
			float theta = U * (PI * 2);

			float x = center.x + radius.x * (cosf(theta) * sinf(phi));
			float y = center.y + radius.y * cosf(phi);
			float z = center.z + radius.z * sinf(theta) * sinf(phi);

			coords.vertex.push_back(glm::vec3(x, y, z));
			coords.normals.push_back(glm::vec3(x, y, z));

			coords.texCoords.push_back(glm::vec2(U, V));
		}
	}

	for (auto i = 0; i < slices * stacks + slices; ++i) {

		coords.indices.push_back(i);
		coords.indices.push_back(i + slices + 1);
		coords.indices.push_back(i + slices);

		coords.indices.push_back(i + slices + 1);
		coords.indices.push_back(i);
		coords.indices.push_back(i + 1);
	}
	return coords;
}

inline MeshInfo getUnitCirclePoints(const unsigned int &slices = 20) {
	MeshInfo coords{};
	auto step = 2 * PI / slices;
	float sectorAngle;

	for (auto i = 0; i <= slices; i++) {
		sectorAngle = i * step;
		coords.vertex.emplace_back(glm::cos(sectorAngle), glm::sin(sectorAngle), 0);
	}

	return coords;
}

inline MeshInfo getCylinderVertices(const float &height = 5.f, const unsigned int &slices = 20, const Pair<float> radius = {1, 1}, const unsigned int &stacks = 20) {
	MeshInfo coords{};
	auto unitCircle = getUnitCirclePoints(slices);

	for (auto i = 0; i < 2; i++) {
		auto h = -height / 2.f + i * height;
		auto t = 1.f - i;

		for (auto j = 0; j <= slices; j++) {
			auto uv = unitCircle.vertex[j];

			// Vertices
			coords.vertex.emplace_back(uv.x * radius.x, uv.y * radius.y, h);
			// Normals
			coords.normals.emplace_back(uv);
			// Texture
			coords.texCoords.emplace_back((float)j / slices, t);
		}
	}

	auto baseCenterIndex = unitCircle.vertex.size();
	auto topCenterIndex = baseCenterIndex + slices + 1;

	int factor = -1;
	for (int i = 0; i < 2; i++) {
		auto h = (height / 2.f) * factor;
		auto nz = factor;

		coords.vertex.push_back({0, 0, h});
		coords.normals.push_back({0, 0, nz});
		coords.texCoords.push_back({0.5f, 0.5f});

		for (int j = 0; j <= slices; j++) {
			auto uv = unitCircle.vertex[j];

			// Vertices
			coords.vertex.push_back({uv.x * radius.x, uv.y * radius.y, h});
			// Normals
			coords.normals.push_back({0, 0, nz});
			// Texture
			coords.texCoords.push_back({-uv.x * 0.5f + 0.5f, -uv.y * 0.5f + 0.5f});
		}
		factor *= -1;
	}

	// indices
	auto k1 = 0;
	auto k2 = slices + 1;

	for (auto i = 0; i < slices; i++, k1++, k2++) {
		coords.indices.emplace_back(k1);
		coords.indices.emplace_back(k1 + 1);
		coords.indices.emplace_back(k2);

		coords.indices.emplace_back(k2);
		coords.indices.emplace_back(k1 + 1);
		coords.indices.emplace_back(k2 + 1);
	}

	for (int i = 0, k = baseCenterIndex + 1; i < slices; i++, k++) {
		coords.indices.push_back(baseCenterIndex);
		coords.indices.push_back((i < slices - 1) ? k + 1 : baseCenterIndex + 1);
		coords.indices.push_back(k);
	}

	for (int i = 0, k = topCenterIndex + 1; i < slices; i++, k++) {
		coords.indices.push_back(topCenterIndex);
		coords.indices.push_back(k);
		coords.indices.push_back((i < slices - 1) ? k + 1 : topCenterIndex + 1);
	}

	return coords;
}

inline MeshInfo getThorusVertices(const unsigned int &side = 20, const unsigned int &sector = 20) {
	MeshInfo coords{};
	float R = 1, r = 0.5; // radius and minorRadius

	float lenghtInv = 1.f / r;
	float sectorStep = 2 * PI / sector;
	float sideStep = 2 * PI / side;

	float sideAngle, sectorAngle, xy, x, y, z;
	float s, t; // Texture Coords

	// Calc The Vertices
	for (auto i = 0; i <= side; ++i) {
		sideAngle = PI - i * sideStep;
		xy = r * cosf(sideAngle);
		z = r * sinf(sideAngle);

		// Loop Through slices
		for (auto j = 0; j <= sector; ++j) {
			sectorAngle = j * sectorStep;

			x = xy * cosf(sectorAngle);
			y = xy * sinf(sectorAngle);

			coords.normals.push_back(glm::vec3{x, y, z} * lenghtInv);

			x += R * cosf(sectorAngle);
			y += R * sinf(sectorAngle);
			coords.vertex.push_back(glm::vec3(x, y, z));

			// Texture Coord
			s = (float)j / sector;
			t = (float)i / side;
			coords.texCoords.push_back(glm::vec2(s, t));
		}
	}

	unsigned int k1, k2;
	for (int i = 0; i < side; ++i) {
		k1 = i * (sector + 1); // beginning of current side
		k2 = k1 + sector + 1;  // beginning of next side
		for (int j = 0; j < sector; ++j, ++k1, ++k2) {
			coords.indices.push_back(k1);
			coords.indices.push_back(k2);
			coords.indices.push_back(k1 + 1);
			coords.indices.push_back(k1 + 1);
			coords.indices.push_back(k2);
			coords.indices.push_back(k2 + 1);
		}
	}
	return coords;
}

inline std::vector<glm::vec4> getColorVector(const glm::vec4 &color, const size_t &size) {
	auto res = std::vector<glm::vec4>{};
	for (size_t i = 0; i < size; i++) {
		res.push_back(color);
	}
	return res;
}
