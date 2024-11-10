#pragma once

#include "Layer.hpp"
#include "Utils.hpp"

#include "../ECS/EntityManager.hpp"
#include "../Platform/MeshFactory.hpp"

#include <mutex>
#include <vector>

namespace bolt {
	namespace scene {
		enum SceneType {
			SCENE_2D,
			SCENE_3D
		};

		void updatePerspective(const f32& fov, const f32& ratio, const f32& near, const f32& far);

		void updateOrtho(const f32& left, const f32& right, const f32& bottom, const f32& up);

		void updateTextProj(const f32& left, const f32& right, const f32& bottom, const f32& up);
	}

	class Scene {
	private:
		inline static Shared<Scene> s_pointer = nullptr;
		inline static std::mutex s_mutex;

		std::vector<u32> m_entities{};
		Scene() = default;

	public:
		Scene(Scene& other) = delete;

		void operator=(const Scene& other) = delete;

		inline static Shared<Scene> instance() {
			std::lock_guard<std::mutex> lock(s_mutex);
			if (s_pointer == nullptr) {
				Shared<Scene> copy(new Scene());
				copy.swap(s_pointer);
			}

			return s_pointer;
		}

		inline u32 getEntitiesCount() const { return static_cast<u32>(this->m_entities.size()); }

		inline std::vector<u32> getEntities() const { return this->m_entities; }

		inline void addEntity(u32 entity) { this->m_entities.push_back(entity); }

		inline void destroy() { this->m_entities.clear(); }
	};

	class SceneLayer : public Layer {
	private:
	public:
		SceneLayer() = default;

		virtual ~SceneLayer() override = default;

		virtual void onAttach() override;

		inline virtual void onDetach() override {
			Scene::instance()->destroy();
		}

		virtual void onRender() override;
	};
} // namespace Bolt
