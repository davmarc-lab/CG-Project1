#pragma once

#include "../Application/Application.hpp"

#include "../Core/Math.hpp"
#include "../Core/RenderApi.hpp"
#include "../Core/Utils.hpp"
#include "Component.hpp"
#include "EntityManager.hpp"

#include "../Core/Scene.hpp"
#include "../Graphic/Camera/Camera.hpp"

#include "../../../Bolt-Graphics/include/glad/glad.h"

namespace bolt {
	inline constexpr auto base = mat4(1.f);

	namespace systems {
		namespace transform {
			inline void updateEntityPosition(u32 id, const vec3 &pos) {
				auto e = EntityManager::instance()->getEntityComponent<Transform>(id);
				if (e != nullptr) {
					e->addPosition(pos);
				}
			}

			inline void updateEntityRotation(u32 id, const vec3 &rot) {
				auto e = EntityManager::instance()->getEntityComponent<Transform>(id);
				if (e != nullptr)
					e->setRotation(rot);
			}

			inline void updateModelMatrix(u32 id) {
				auto model = EntityManager::instance()->getEntityComponent<Transform>(id);

				if (!model->isModelMatrixEnable())
					return;
				if (!model->isDirty())
					return;

				const auto pos = translate(base, model->getPosition());
				const auto scl = scale(base, model->getScale());
				model->setQuaternion(quat(model->getRotation()));
				const auto rot = toMat4(model->getQuaternion());

				model->setModelMatrix(pos * scl * rot);
			}

			inline void updateAllModelMatrix() {
				auto ids = EntityManager::instance()->getEntitiesFromComponent<Transform>();
				for (const auto id : ids) {
					updateModelMatrix(id);
				}
			}
		} // namespace transform

		namespace render {
			inline void drawElement(u32 id) {
				const auto mesh = EntityManager::instance()->getEntityComponent<Mesh>(id);
				const auto vao = mesh->vao;
				const auto model = EntityManager::instance()->getEntityComponent<Transform>(id);
				const auto shader = EntityManager::instance()->getEntityComponent<ShaderComponent>(id)->shader.get();

				shader->use();
				if (Application::getSceneType() == scene::SCENE_3D) {
					shader->setMat4("view", standardCamera.getViewMatrix());
				}
				shader->setMat4("model", model->getModelMatrix());

				RenderApi::instance()->getRenderer()->drawArraysTriangles(vao, mesh->vertices.size());
				vao.unbind();
			}

			inline void drawElementIndexed(u32 id) {
				const auto mesh = EntityManager::instance()->getEntityComponent<Mesh>(id);
				const auto model = EntityManager::instance()->getEntityComponent<Transform>(id);
				const auto shader = EntityManager::instance()->getEntityComponent<ShaderComponent>(id)->shader.get();

				shader->use();
				if (Application::getSceneType() == scene::SCENE_3D) {
					shader->setMat4("view", standardCamera.getViewMatrix());
				}
				shader->setMat4("model", model->getModelMatrix());

				RenderApi::instance()->getRenderer()->drawElementsTriangles(mesh->vao, mesh->indices.size());
			}

			inline void drawAllMeshes() {
				const auto meshes = EntityManager::instance()->getEntitiesFromComponent<Mesh>();
				for (const auto id : meshes) {
					const auto mesh = EntityManager::instance()->getEntityComponent<Mesh>(id);
					const auto model = EntityManager::instance()->getEntityComponent<Transform>(id);
					const auto shader = EntityManager::instance()->getEntityComponent<ShaderComponent>(id)->shader.get();

					shader->use();
					if (Application::getSceneType() == scene::SCENE_3D) {
						shader->setMat4("view", standardCamera.getViewMatrix());
					}
					shader->setMat4("model", model->getModelMatrix());

					mesh->render.call();
				}
			}
		} // namespace render
	} // namespace systems
} // namespace bolt
