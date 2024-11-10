#pragma once

#include "../../Core/Layer.hpp"
#include "../../Core/Math.hpp"
#include "../../Core/Utils.hpp"

#include "../Buffer/VertexArray.hpp"
#include "../Buffer/VertexBuffer.hpp"

#include <map>

namespace bolt {
	struct Character {
		u32 textureId;
		ivec2 size;
		ivec2 bearing;
		i64 advance;
	};

	struct TextSettings {
		std::string fontFace = "./resources/fonts/arial.ttf";
		u32 pixelWidth = 0;
		u32 pixelHeight = 48;
		u32 numChars = 128;
	};

	struct TextHelper {
		std::string text = "Simple Text";
		vec2 position = {0, 0};
		f32 scale = 1.f;
		vec3 color = {1, 0, 0};
	};

	class Text {
	public:
		Text(const TextHelper &config) :
			m_config(config) {}

		~Text() = default;

		void init();

        inline void bindVAO() { this->m_vao.bind(); }

        inline void bindVBO() { this->m_vbo.bind(); }

        inline u32 getVboId() { return this->m_vbo.getId(); }

		inline void setText(std::string text) { this->m_config.text = std::move(text); }

		inline std::string getText() const { return this->m_config.text; }

		inline void setColor(const vec4 &color) { this->m_config.color = color; }

		inline vec3 getColor() const { return this->m_config.color; }

		inline vec2 getPosition() const { return this->m_config.position; }

		inline void setPosition(const vec2 &pos) { this->m_config.position = pos; }

		inline f32 getScale() const { return this->m_config.scale; }

		inline void setScale(const f32 &scale) { this->m_config.scale = scale; }

	private:
		VertexArray m_vao{};
		VertexBuffer m_vbo{};

		TextHelper m_config{};
        b8 m_created = false;
	};

	class TextManager : public Layer {
	public:
		TextManager() = default;

		TextManager(TextSettings settings) :
			m_settings(std::move(settings)) {}

        void addText(Shared<Text> text) {
            text->init();
            this->m_text.push_back(text);
        }

		virtual void onAttach() override;

		virtual void onDetach() override;

		virtual void onRender() override;

	private:
		TextSettings m_settings{};

		std::map<u64, Character> m_characters{};

		std::vector<Shared<Text>> m_text{};
	};
} // namespace bolt
