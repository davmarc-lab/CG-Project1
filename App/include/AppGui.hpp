#pragma once

#include "../../Opengl-Core/include/Core.hpp"
#include "Scene.hpp"

using namespace ogl;

class ImGuiStats : public ImGuiPanel {
public:
	ImGuiStats() :
		ImGuiPanel("Stats", 0) {}

	virtual ~ImGuiStats() = default;

private:
};

class ImGuiScene : public ImGuiPanel {
public:
	ImGuiScene(Scene &scene) :
		ImGuiPanel("Scene", 0), m_scene(scene) {}

	virtual ~ImGuiScene() override = default;

private:
	Scene &m_scene;
};

class ImGuiModel : public ImGuiPanel {
public:
	void changeObserver(const unsigned int &id) { this->m_id = id; }

	unsigned int getCurrentId() const { return this->m_id; }

	ImGuiModel(const unsigned int &id = 0) :
		m_id(id),
		ImGuiPanel("Model", 0) {}

	virtual ~ImGuiModel() override = default;

private:
	unsigned int m_id = 0;
};
