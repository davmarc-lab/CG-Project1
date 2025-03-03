#include "../../include/Text/TextManager.hpp"
#include "../../include/Texture/Texture.hpp"

#include "../../include/Shader/Shader.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace ogl {

	Shared<ShaderProgram> textShader = CreateShared<ShaderProgram>("textVertShader.glsl", "textFragShader.glsl");

	void Text::init() {
		if (this->m_created)
			return;
		this->m_vao.onAttach();
		this->m_vbo.onAttach();
		this->m_vao.bind();
		this->m_vbo.setup(NULL, sizeof(float) * 6 * 4, GL_DYNAMIC_DRAW);
		this->m_vao.linkAttribFast(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		this->m_created = true;
	}

	void TextManager::onAttach() {
		FT_Library ft;
		if (FT_Init_FreeType(&ft)) {
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library\n";
			exit(EXIT_FAILURE);
		}

		FT_Face face;
		if (FT_New_Face(ft, this->m_settings.fontFace.c_str(), 0, &face)) {
			std::cout << "ERROR::FREETYPE: Failed to load font\n";
			exit(EXIT_FAILURE);
		}

		FT_Set_Pixel_Sizes(face, this->m_settings.pixelWidth, this->m_settings.pixelHeight);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		TextureParams param{};
		param.target = GL_TEXTURE_2D;
		param.internalFormat = GL_RED;
		param.border = 0;
		param.format = GL_RED;
		param.dataType = GL_UNSIGNED_BYTE;
		for (unsigned char c = 0; c < this->m_settings.numChars; c++) {
			if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph\n";
				continue;
			}

			Texture texture = Texture(param, face->glyph->bitmap.width, face->glyph->bitmap.rows);
			texture.onAttach();
			texture.createTexture2D(face->glyph->bitmap.buffer);
			texture.setTexParameteri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			texture.setTexParameteri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			texture.setTexParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			texture.setTexParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			this->m_characters.insert_or_assign(c, Character{texture.getId(), glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows), glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top), face->glyph->advance.x});
		}
		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		textShader->createShaderProgram();
	}

	void TextManager::onRender() {
		textShader->use();
		for (auto t : this->m_text) {
			textShader->setVec3("textColor", t->getColor());
			glActiveTexture(GL_TEXTURE0);
			t->bindVAO();

			auto x = t->getPosition().x;
			auto y = t->getPosition().y;
			auto scale = t->getScale();

			std::string::const_iterator c;
			auto s = t->getText();
			for (c = s.begin(); c != s.end(); ++c) {
				auto ch = this->m_characters[*c];

				float xpos = x + ch.bearing.x * scale;
				float ypos = y - (ch.size.y - ch.bearing.y) * scale;

				float w = ch.size.x * scale;
				float h = ch.size.y * scale;

				float vertices[6][4] = {
					{xpos, ypos + h, 0.0f, 0.0f},
					{xpos, ypos, 0.0f, 1.0f},
					{xpos + w, ypos, 1.0f, 1.0f},

					{xpos, ypos + h, 0.0f, 0.0f},
					{xpos + w, ypos, 1.0f, 1.0f},
					{xpos + w, ypos + h, 1.0f, 0.0f}};

				glBindTexture(GL_TEXTURE_2D, ch.textureId);
				t->bindVBO();
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				glDrawArrays(GL_TRIANGLES, 0, 6);

				x += (ch.advance >> 6) * scale;
			}

			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	Shared<Text> TextManager::addText(const TextHelper &config) {
		auto t = CreateShared<Text>(config);
		t->init();
		this->m_text.push_back(t);
		std::cout << this->m_text.size() << "\n";
		return t;
	}
} // namespace ogl
