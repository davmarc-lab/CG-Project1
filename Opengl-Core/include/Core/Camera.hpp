#pragma once

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

#include "../Core/Utils.hpp"

/**
 * @namespace ogl
 */
namespace ogl {
	inline const float YAW = -90.f;
	inline const float PITCH = 0.f;
	inline const float ROLL = 0.f;
	inline const float SPEED = 0.2f;
	inline const float SENSITIVITY = 0.02f;
	inline const float ZOOM = 45.f;
	inline const glm::vec3 POSITION = glm::vec3(0, 0, 3);
	inline const float TB_SPEED = 20.f;

    /**
     * @brief This data structure contains all camera vectors.
     */
	struct CameraVectors {
        /// camera position vector
		glm::vec3 cameraPos = POSITION;
        /// camera front vector
		glm::vec3 cameraFront = glm::vec3(0, 0, -1);
        /// camera up vector
		glm::vec3 cameraUp = glm::vec3(0, 1, 0);
        /// camera right vactor
		glm::vec3 cameraRight = glm::vec3(1, 0, 0);
        /// camera direction vector
		glm::vec3 cameraDirection = glm::vec3(0);
        /// camera target vector
		glm::vec3 cameraTarget = glm::vec3(0);
	};

    /**
     * @brief This data structure contains basic camera information.
     */
	struct CameraInfo {
        /// camera velocity
		float speed = SPEED;
        /// camera sensitivity
		float sensitivity = SENSITIVITY;
        /// camera zoom
		float zoom = ZOOM;
        /// camera aspect ratio
		float ratio = 1;
        /// camera trackball speed
		float tbSpeed = TB_SPEED;
	};

    /**
     * @brief This data structure contains all camera rotation angles.
     */
	struct CameraRotation {
        /// camera yaw
		float yaw = YAW;
        /// camera pitch
		float pitch = PITCH;
        /// camera roll
		float roll = ROLL;
        /// camera quaternion
		glm::quat rotQuat{};
	};

    /**
     * @brief Creates a virtual camera based on projection matrix.
     * It can be used for both 2D and 3D scenes: it calculates the Projection * View matrix.
     *
     * This class also provide methods to manage position, mouse sensitivity and
     * camera rotation.
     * 
     * @note The projection matrix must be updated everytime the screen
     * size is changed.
     */
	class Camera {
	public:
        /**
         * @brief Retrieves the Projection * View matrix.
         *
         * @return Projection * View matrix
         */
		glm::mat4 getViewProjMatrix() const;

        /**
         * @brief Retrieves the Projection matrix.
         *
         * @return Projection matrix
         */
		inline glm::mat4 getProjMatrix() const { return this->m_proj; }

        /**
         * @brief Retrieves the View matrix.
         *
         * @return View matrix
         */
		inline glm::mat4 getViewMatrix() const { return this->m_view; }

        /**
         * @brief Updates the Proj * View matrix by using orthographic projection
         * and it doesn't affect camera rotation.
         *
         * @note The View matrix used is of type `glm::mat4(1)`
         *
         * @param left values
         * @param right values
         * @param bot bot values
         * @param top top values
         */
		inline void updateOrthoProjection(const float &left, const float &right, const float &bot, const float &top) {
			this->m_proj = glm::ortho(left, right, bot, top);
			this->m_view = glm::mat4(1);
			this->m_viewProj = this->m_proj * this->m_view;
		}

        /**
         * @brief Updates Projection * View matrix by using perspective projection.
         * This method uses the given width and height to calculate the aspect ratio.
         *
         * @param fov camera fov
         * @param width viewport width
         * @param height viewport height
         * @param near camera near plane
         * @param far camera far plane
         */
		inline void updatePerspProjection(const float &fov, const float &width, const float &height, const float &near, const float &far) {
			this->m_proj = glm::perspective(glm::radians(fov), width / height, near, far);
			this->m_info.ratio = width / height;
			this->updateCameraVectors();
		}

        /**
         * @brief Updates Projection * View matrix by using perspective projection.
         *
         * @param fov camera fov
         * @param ratio viewport aspect ratio
         * @param near camera near plane
         * @param far camera far plane
         */
		inline void updatePerspProjection(const float &fov, const float &ratio, const float &near, const float &far) {
			this->m_proj = glm::perspective(glm::radians(fov), ratio, near, far);
			this->m_info.ratio = ratio;
			this->updateCameraVectors();
		}

        /**
         * @brief Moves the camera on the given axis of an offset equal to the camera velocity.
         *
         * @param axis direction vector
         */
		void moveCamera(const glm::vec3 &axis);

        /**
         * @brief Calculates the camera rotation by using mouse current movement offset.
         *
         * @param xoffset mouse x-axis offset
         * @param yoffset mouse y-axis offset
         * @param constrainPitch bool that indicate if the camera should lock y-axis maximum value
         */
		void processMouseMovement(const float &xoffset, const float &yoffset, const bool &constrainPitch = true);

		inline void setCameraFront(glm::vec3 vec) { this->m_vectors.cameraFront = vec; }

		inline glm::vec3 getCameraFront() const { return this->m_vectors.cameraFront; }

		inline glm::vec3 getCameraUp() const { return this->m_vectors.cameraUp; }

        /**
         *
         */
		inline void setCameraVelocity(const float velocity) { this->m_info.speed = velocity; }

        /**
         * @brief Retrieves camera velocity.
         *
         * @return camera velocity
         */
		inline float getCameraVelocity() const { return this->m_info.speed; }

		inline glm::vec3 getCameraRight() const { return this->m_vectors.cameraRight; }

        /**
         * @brief Retrieves camera current position.
         *
         * @return camera position
         */
		inline glm::vec3 getCameraPosition() const { return this->m_vectors.cameraPos; }

        /**
         * @brief Updates current camera position and recalculates its vectors.
         *
         * @param position camera new position
         */
		void setCameraPosition(const glm::vec3 &position);

        /**
         * @brief Retrieves camera rotation velocity.
         *
         * @return camera sensitivity
         */
		inline float getMouseSensitivity() const { return this->m_info.sensitivity; }

        /**
         * @brief Updates camera rotation velocity.
         *
         * @param val camera sensitivity
         */
		inline void setMouseSensitivity(float val) { this->m_info.sensitivity = val; }

        /**
         * @brief Retrieves camera zoom.
         *
         * @return camera zoom
         */
		inline float getCameraZoom() const { return this->m_info.zoom; }

        /**
         * @brief Updates camera zoom.
         *
         * @param zoom camera zoom
         */
		inline void setCameraZoom(const float zoom) {
			this->m_info.zoom = zoom;
			this->updatePerspProjection(this->m_info.zoom, this->m_info.ratio, 0.1f, 100.f);
		}

        /**
         * @brief Retrieves `CameraRotation` values.
         *
         * @return camera rotation values
         */
		inline CameraRotation getCameraRotation() const { return this->m_rotation; }

        /**
         * @brief Updates camera current yaw value.
         *
         * @param yaw camera yaw
         */
		inline void setCameraYaw(const float &yaw) {
			this->m_rotation.yaw = yaw;
			this->updateCameraVectors();
		}

        /**
         * @brief Updates camera current pitch value.
         *
         * @param pitch camera pitch
         */
		inline void setCameraPitch(const float &pitch) {
			this->m_rotation.pitch = pitch;
			this->updateCameraVectors();
		}

        /**
         * @brief Retrieves trackball speed of the camera.
         *
         * @return trackball speed
         */
		inline float getTrackballSpeed() { return this->m_info.tbSpeed; }

        /**
         * @brief Updates trackball speed of the camera.
         *
         * @param speed trackball speed
         */
		inline void setTrackballSpeed(const float speed) { this->m_info.tbSpeed = speed; }

		inline glm::vec3 getCameraDirection() const { return this->m_vectors.cameraDirection; }

		inline void setCameraDirection(glm::vec3 dir) { this->m_vectors.cameraDirection = dir; }

		inline glm::vec3 getCameraTarget() const { return this->m_vectors.cameraTarget; }

		inline void setCameraTarget(glm::vec3 target) { this->m_vectors.cameraTarget = target; }

        /**
         * @brief Camera basic constructor.
         */
		Camera() { this->updateCameraVectors(); }

		~Camera() = default;

	private:
		glm::vec3 m_worldUp = glm::vec3(0, 1, 0);
		CameraVectors m_vectors{};
		CameraInfo m_info{};
		CameraRotation m_rotation{};

		glm::mat4 m_view = glm::mat4(0);
		glm::mat4 m_proj = glm::mat4(0);
		glm::mat4 m_viewProj = glm::mat4(0);

		void updateCameraVectors();
	};

	/**
	 * @brief Data structure used to store world camera information.
	 */
	struct WorldCamera {
		/// world camera ECS id
		unsigned int cameraId;
		/// pointer to ogl::Camera
		Shared<Camera> camera;
		/// size of the collider
		glm::vec3 cameraSize = glm::vec3(1);
		/// trackball movement tolerance
		/// @note This tolerance is used to move the cursor to the opposite side
		/// of the window in case it goes near the window border.
		int tbBorderTolerance = 20;
		/// trackball radius
		float tbRadius = 1.f;
	};
} // namespace ogl
