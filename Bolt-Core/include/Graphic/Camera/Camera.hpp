#pragma once

#include "../../Core/Utils.hpp"
#include "../../Core/Math.hpp"

namespace bolt {
	namespace camera {
		inline const f32 YAW = -90.f;
		inline const f32 PITCH = 0.f;
		inline const f32 SPEED = 0.2f;
		inline const f32 SENSITIVITY = 0.02f;
		inline const f32 ZOOM = 45.f;
		inline const vec3 POSITION = vec3(0, 0, 3);
		// f32 tbSpeed = 20.f;

		struct CameraVectors {
			vec3 cameraPos = POSITION;
			vec3 cameraFront = vec3(0, 0, -1);
			vec3 cameraUp = vec3(0, 1, 0);
			vec3 cameraRight = vec3(1, 0, 0);
			vec3 cameraDirection = vec3(0);
			vec3 cameraTarget = vec3(0);
		};

		struct CameraInfo {
			f32 speed = SPEED;
			f32 sensitivity = SENSITIVITY;
			f32 zoom = ZOOM;
			//f32 tbSpeed = speed;
		};

		struct CameraRotation {
			f32 yaw = YAW;
			f32 pitch = PITCH;
		};
	};

	class Camera {
	private:
		vec3 worldUp = vec3(0, 1, 0);
		camera::CameraVectors vectors;
		camera::CameraInfo info;
		camera::CameraRotation rotation;

		void updateCameraVectors();

	public:
		mat4 getViewMatrix();

		void moveCamera(vec3 position);

		void processMouseMovement(f32 xoffset, f32 yoffset, b8 constrainPitch = true);

		inline void setCameraFront(vec3 vec) { this->vectors.cameraFront = vec; }

		inline vec3 getCameraFront() { return this->vectors.cameraFront; }

		inline vec3 getCameraUp() { return this->vectors.cameraUp; }

		inline void setCameraVelocity(const float velocity) { this->info.speed = velocity; }

		inline float getCameraVelocity() { return this->info.speed; }

		inline vec3 getCameraRight() { return this->vectors.cameraRight; }

		inline vec3 getCameraPosition() { return this->vectors.cameraPos; }

		inline float getMouseSensitivity() { return this->info.sensitivity; }

		inline void setMouseSensitivity(float val) { this->info.sensitivity = val; }

		inline float getCameraZoom() { return this->info.zoom; }

		inline void setCameraZoom(const float zoom) { this->info.zoom = zoom; }

		//inline float getTrackballSpeed() { return this->info.tb_speed; }

		//inline void setTrackballSpeed(const float speed) { this->info.tb_speed = speed; }

		inline vec3 getCameraDirection() { return this->vectors.cameraDirection; }

		inline void setCameraDirection(vec3 dir) { this->vectors.cameraDirection = dir; }

		inline vec3 getCameraTarget() { return this->vectors.cameraTarget; }

		inline void setCameraTarget(vec3 target) { this->vectors.cameraTarget = target; }

		Camera() { this->updateCameraVectors(); }

		~Camera() = default;
	};

	inline Camera standardCamera{};
};
