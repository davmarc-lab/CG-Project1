#pragma once

#include <cstddef>

#include "../../../Opengl-Core/include/Core.hpp"
#include "Component.hpp"
#include "Ett.hpp"

namespace systems {
	namespace transform {
		void updatePosition(const unsigned int &id, const glm::vec3 &position);
		void updateScale(const unsigned int &id, const glm::vec3 &scale);
		void updateRotation(const unsigned int &id, const glm::vec3 &rotation);

		void addPosition(const unsigned int &id, const glm::vec3 &offset);
		void addScale(const unsigned int &id, const glm::vec3 &offset);
		void addRotation(const unsigned int &id, const glm::vec3 &offset);

		void updateModelMatrix(const unsigned int &id);

		void updateAllModelMatrix();

	} // namespace transform

	namespace render {
		void renderAllMeshes();
	}
} // namespace systems
