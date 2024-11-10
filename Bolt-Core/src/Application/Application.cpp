#include "../../include/Application/Application.hpp"

#include "../../include/Core/InputManager.hpp"
#include "../../include/Core/LayerManager.hpp"
#include "../../include/Core/RenderApi.hpp"

#include "../../include/ECS/EntityManager.hpp"
#include "../../include/ECS/System.hpp"

#include "../../include/Graphic/Buffer/UniformBuffer.hpp"

#include "../../include/Core/Timestep.hpp"

#include "../../include/Graphics.hpp"

void bolt::Application::run() {
	const auto lm = LayerManager::instance();

	using namespace bmath;

	const auto rd = RenderApi::instance();
	rd->init(config::RenderApiConfig::render_opengl);

	const auto im = InputManager::instance();

	const auto ed = EventDispatcher::instance();

	lm->addLayersFromStack();

	if (s_settings.enableCollisions) {
		// enable collision detection
		ed->subscribe(events::loop::LoopBeforeRender, [](auto p) {});
	}

	if (s_settings.defaultCameraMovement && this->getSceneType() == scene::SCENE_3D) {
		ed->subscribe(events::loop::LoopInput, [&im](auto p) {
			if (im->isKeyPressed(GLFW_KEY_W)) {
				standardCamera.moveCamera(standardCamera.getCameraVelocity() * standardCamera.getCameraFront());
			}

			if (im->isKeyPressed(GLFW_KEY_S)) {
				standardCamera.moveCamera(standardCamera.getCameraVelocity() * -standardCamera.getCameraFront());
			}

			if (im->isKeyPressed(GLFW_KEY_D)) {
				standardCamera.moveCamera(standardCamera.getCameraVelocity() * standardCamera.getCameraRight());
			}

			if (im->isKeyPressed(GLFW_KEY_A)) {
				standardCamera.moveCamera(standardCamera.getCameraVelocity() * -standardCamera.getCameraRight());
			}

			if (im->isKeyPressed(GLFW_KEY_SPACE)) {
				standardCamera.moveCamera(standardCamera.getCameraVelocity() * standardCamera.getCameraUp());
			}

			if (im->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
				standardCamera.moveCamera(standardCamera.getCameraVelocity() * -standardCamera.getCameraUp());
			}
		});
	}

	ed->subscribe(events::window::WindowCloseEvent, [this](auto p) {
		this->closeApplication();
	});

	Timer::instance()->start();

	// TEST START
	/*
		{
			bmat2 a = bmat2({1, 2}, {3, 4});
			bmat2 b = bmat2({7, 5}, {6, 8});

			auto main = Timer::instance()->getTime();
			for (int i = 0; i < 10; i++) {
				auto start = Timer::instance()->getTime();
				std::cout << dump(a * b) << "\n";
				std::cout << Timer::instance()->getTime() - start << "\n";
			}
			std::cout << "FINAL: " << Timer::instance()->getTime() - main << "\n";

		}

		{
			std::cout << "\n\n\n\n";
			mat2 a = mat2({1, 3}, {2, 4});
			mat2 b = mat2({7, 6}, {5, 8});

			auto main = Timer::instance()->getTime();
			for (int i = 0; i < 10; i++) {
				auto start = Timer::instance()->getTime();
				std::cout << to_string(a * b) << "\n";
				std::cout << Timer::instance()->getTime() - start << "\n";
			}
			std::cout << "FINAL: " << Timer::instance()->getTime() - main << "\n";
		}

	auto pw = CreateShared<PhysicsWorld>();
	lm->addLayer(pw);

	// using a loop generic event to add entities to the physic world
	ed->subscribe(events::loop::LoopGeneric, [&pw](auto &&p) {
		pw->addEntity(EntityManager::instance()->getCurrentId() - 1);
	});
	*/
	// TEST END

	while (!this->shouldClose()) {
		auto e = Event();
		ed->post(events::loop::LoopInput);
		lm->execute([e](const Shared<Layer> &l) { l->onEvent(e); });


		ed->post(events::loop::LoopUpdate);
		lm->execute([](const Shared<Layer> &l) { l->onUpdate(); });

		ed->post(events::loop::LoopBeforeRender);

		// Before rendering operations
		lm->execute([](const Shared<Layer> &l) { l->begin(); });
		lm->execute([](const Shared<Layer> &l) { l->onRender(); });

		lm->execute([](const Shared<Layer> &l) { l->end(); });
		// After rendering operations
	}
	lm->execute([](const Shared<Layer> &l) { l->onDetach(); });
}
