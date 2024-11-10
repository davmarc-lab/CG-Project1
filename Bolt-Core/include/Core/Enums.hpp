#pragma once

#include "Utils.hpp"

namespace bolt {
	namespace config {
		typedef u8 MeshConfig;

		/// Configs for mesh creation.
		enum MeshConfig_ {
			mesh_colors         = 1 << 0,
			mesh_texture        = 1 << 1,
			mesh_indices        = 1 << 2,
			mesh_normals        = 1 << 3,
			mesh_default_shader = 1 << 4,
		};

		inline BT_CONSTEXPR MeshConfig cubeConfig = config::mesh_colors;
		inline BT_CONSTEXPR MeshConfig squareConfig = config::mesh_colors;
		inline BT_CONSTEXPR MeshConfig circleConfig = config::mesh_colors;

		enum MeshShape {
			shape_square,
			shape_circle,
			shape_cube,
			shape_sphere,
			shape_custom,
		};

		/// Congfigs for render api.
		enum RenderApiConfig {
			render_unknonw,
			render_opengl,
		};
	} // namespace config

	namespace ecs {
		BT_CONSTEXPR u32 MAX_ENTITIES = 1024;
	}
} // namespace bolt
