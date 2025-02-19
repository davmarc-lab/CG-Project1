#pragma once

#include "../../Opengl-Core/include/Core.hpp"

using namespace ogl;

class ImGuiStats : public ImGuiPanel {
public:
	virtual void onRender() override;

	ImGuiStats() :
		ImGuiPanel("Stats", 0) {}

	virtual ~ImGuiStats() = default;

private:
};

class ImGuiDebug : public ImGuiPanel {
public:
	bool isBoundingBoxVisible() const { return this->m_boundingBox; }

	void showBoundingBox(const bool &val = true) { this->m_boundingBox = val; }

	ImGuiDebug() :
		ImGuiPanel("Debug", 0) {
		EventManager::instance()->post(event::shader::INIT_DEFAULT_SHADER);
	}

	virtual ~ImGuiDebug() = default;

private:
	bool m_boundingBox = false;
};

class ImGuiScene : public ImGuiPanel {
public:
	ImGuiScene() :
		ImGuiPanel("Scene", 0) {}

	virtual ~ImGuiScene() override = default;
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
