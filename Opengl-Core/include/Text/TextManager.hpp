#pragma once

#include "../../include/Buffer/VertexArray.hpp"
#include "../../include/Buffer/VertexBuffer.hpp"
#include "../../include/Core/Layer.hpp"
#include "../../include/Core/Utils.hpp"
#include "../../include/Shader/Shader.hpp"

#include <glm/glm.hpp>
#include <map>
#include <vector>

namespace ogl {
	struct TextSettings {
		std::string fontFace = "./resources/fonts/arial.ttf";
		unsigned int pixelWidth = 0;
		unsigned int pixelHeight = 48;
		unsigned int numChars = 128;
	};

	struct TextHelper {
		std::string text = "Simple Text";
		glm::vec2 position = {0, 0};
		float scale = 1.f;
		glm::vec3 color = {1, 0, 0};
	};

	class Text {
	public:
		void init();

		inline void bindVAO() { this->m_vao.bind(); }

		inline void bindVBO() { this->m_vbo.bind(); }

		// inline unsigned int getVboId() { return this->m_vbo.getId(); }

		inline void setText(std::string text) { this->m_config.text = std::move(text); }

		inline std::string getText() const { return this->m_config.text; }

		inline void setColor(const glm::vec4 &color) { this->m_config.color = color; }

		inline glm::vec3 getColor() const { return this->m_config.color; }

		inline glm::vec2 getPosition() const { return this->m_config.position; }

		inline void setPosition(const glm::vec2 &pos) { this->m_config.position = pos; }

		inline float getScale() const { return this->m_config.scale; }

		inline void setScale(const float &scale) { this->m_config.scale = scale; }

		Text() = delete;
		Text(const TextHelper &config) :
			m_config(config) {}

	private:
		TextHelper m_config{};

		VertexArray m_vao{};
		VertexBuffer m_vbo{};

		bool m_created = false;
	};

	class TextManager : public Layer {

		struct Character {
			unsigned int textureId;
			glm::ivec2 size;
			glm::ivec2 bearing;
			long long advance;
		};

	public:
		virtual void onAttach() override;

		virtual void onRender() override;

		Shared<Text> addText(const TextHelper &config);

		TextManager(TextManager &other) = delete;

		void operator=(const TextManager &other) = delete;

		/*
		 * Retrieves the instance of the TextManager if it's not created.
		 *
		 * @return `TextManager` unique object.
		 */
		inline static Shared<TextManager> instance() {
			if (s_pointer == nullptr) {
				Shared<TextManager> copy(new TextManager());
				copy.swap(s_pointer);
			}

			return s_pointer;
		}

		virtual ~TextManager() override = default;

	private:
		TextManager() = default;

		inline static Shared<TextManager> s_pointer = nullptr;

		std::map<unsigned long long, Character> m_characters{};
		std::vector<Shared<Text>> m_text{};

		TextSettings m_settings{};
	};
} // namespace ogl
