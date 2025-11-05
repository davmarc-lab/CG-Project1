#pragma once

#include "../../Opengl-Core/include/Core.hpp"

#include <glm/glm.hpp>
#include <string>
#include <vector>

/**
 * @brief Helper data structure to create meshes.
 */
struct BasicInfo {
	/// mesh position
	glm::vec3 position{};
	/// mesh scale
	glm::vec3 scale{1, 1, 1};
	/// mesh rotation
	glm::vec3 rotation{};
};

/**
 * @brief Data strucure to store mesh vertices used while creating the mesh.
 */
struct EntityVertex {
	EntityVertex() = default;

	/**
	 * @brief Simple constructor.
	 *
	 * @param coords the mesh vertex coords vector
	 * @param colors the mesh color vector
	 * @param indices the mesh inices vector
	 */
	EntityVertex(const std::vector<glm::vec3> &coords, const std::vector<glm::vec4> &colors, const std::vector<unsigned int> &indices) :
		coords(coords), colors(colors), indices(indices) {}

	/// mesh vertex coords vector
	std::vector<glm::vec3> coords{};
	/// mesh color vector
	std::vector<glm::vec4> colors{};
	/// mesh indices vector
	std::vector<unsigned int> indices{};
};

const glm::vec3 ENEMY_SLIME_SIZE{120, 220, 1};
const glm::vec3 SLIME_LEYE_OFFSET{-13, 0, 0};
const glm::vec3 SLIME_REYE_OFFSET{10, 0, 0};
const glm::vec3 SLIME_EYE_SCALE{10, 15, 1};

/**
 * @brief Helper data structure to store enemy data.
 */
struct EnemyInfo {
	/// time the enemy last shoot
	float lastShoot = 0;
	/// shoot cooldown time
	float cooldown = 1;
	/// enemy touch damage
	float damage = 20;
	/// enemy health
	float hp = 100;
	/// enemy gun range
	float range = 400;
};

const EnemyInfo ENEMY_FLY_INFO{0, 2, 30, 50, 600};
const EnemyInfo ENEMY_SLIME_INFO{0, 1, 10, 100, 600};

const glm::vec3 PROJ_SIZE = {10, 10, 1};
const glm::vec4 PROJ_COLOR = {0, 1, 0, 1};
const float PROJ_VEL = 5;
const float PROJ_RANGE = 400.f;

/**
 * @brief Data structure to create a projectile.
 */
struct ProjInfo {
	/// projectile health damage
	float damage = 20;
	/// projectile range distance
	float range = PROJ_RANGE;
	/// projectile color
	glm::vec4 color = PROJ_COLOR;
};

/**
 * @brief Creates a background square with no bounding box.
 *
 * @param info the mesh helper data
 * @param color the backgournd color
 *
 * @return the created mesh id
 */
unsigned int factoryBackground(const BasicInfo &info, const glm::vec4 &color = {1, 0, 0, 1});

/**
 * @brief Creates a simple square with the outline component.
 *
 * @param info the mesh helper data
 * @param color the mesh color
 * @param stats the enemy info data
 *
 * @return the created mesh id
 */
unsigned int factorySquare(const BasicInfo &info, const glm::vec4 &color = {1, 0, 0, 1}, const EnemyInfo &stats = {});

/**
 * @brief Creates a simple circle.
 *
 * @param info the mesh helper data
 * @param color the mesh color
 * @param numTriangles the circle triangles number
 * @param center the circle center point in local space
 * @param radius the circle radius values for x and y axis in local space
 * @param stats the enemy info data
 *
 * @return the created mesh id
 */
unsigned int factoryCircle(const BasicInfo &info, const glm::vec4 &color = {1, 0, 0, 1}, const unsigned int &numTriangles = 20, const Pair<float> &center = {0, 0}, const Pair<float> &radius = {1, 1}, const EnemyInfo &stats = {});

/**
 * @brief Creates a projectile with collisions, outline and projectile component.
 *
 * @param info the projectile helper data
 * @param color the projectile color
 * @param projInfo the projectile info data
 * @param direction the projectile direction
 *
 * @return the created mesh id
 */
unsigned int factoryProjectile(const BasicInfo &info, const glm::vec4 &color, const ProjInfo &projInfo, const glm::vec3 &direction);

/**
 * @brief Creates a mesh from a list of coordinates which are used to calculate the final mesh using hermite interpolation.
 *
 * @param info the mesh helper data
 * @param path the file path with the list of points
 * @param color the mesh color
 *
 * @return the created mesh id
 */
unsigned int factoryHermite(const BasicInfo &info, const std::string &path, const glm::vec4 &color = {1, 0, 0, 1});

/**
 * @brief Creates an enemy with health, collision and outline component.
 *
 * @param info the mesh helper data
 * @param color the mesh color
 *
 * @return the created mesh id
 */
unsigned int factoryEnemy(const BasicInfo &info, const glm::vec4 &color);
