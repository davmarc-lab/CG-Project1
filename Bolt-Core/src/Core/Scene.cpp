#include "../../include/Core/Scene.hpp"
#include "../../include/Application/Application.hpp"

#include "../../include/ECS/System.hpp"

namespace bolt {
	namespace scene {
		void updatePerspective(const f32 &fov, const f32 &ratio, const f32 &near, const f32 &far) {
			Application::setProjectionMatrix(perspective(fov, ratio, near, far));
		}

		void updateOrtho(const f32 &left, const f32 &right, const f32 &bottom, const f32 &up) {
			Application::setProjectionMatrix(ortho(left, right, bottom, up));
		}

		void updateTextProj(const f32 &left, const f32 &right, const f32 &bottom, const f32 &up) {
			Application::setTextProjMatrix(ortho(left, right, bottom, up));
		}
	} // namespace scene

	void SceneLayer::onAttach() {
		this->m_attached = true;
	}

	void SceneLayer::onRender() {
		systems::render::drawAllMeshes();
	}
} // namespace bolt
