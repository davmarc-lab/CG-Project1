#include "../../../include/Graphic/Text/TextManager.hpp"

#include "../../../include/Graphics.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>

#include "../../../include/Graphic/Shader/Shader.hpp"
#include "../../../include/Graphic/Texture/Texture.hpp"
#include "../../../include/Application/Application.hpp"

namespace bolt {

	ShaderProgram textShader = ShaderProgram("shader/textVertShader.glsl", "shader/textFragShader.glsl");

	void Text::init() {
		if (this->m_created)
			return;
		this->m_vao.onAttach();
		this->m_vbo.onAttach();
		this->m_vao.bind();
		this->m_vbo.setup(NULL, sizeof(float) * 6 * 4, GL_DYNAMIC_DRAW);
		this->m_vao.linkAttribFast(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		this->m_vbo.unbind();
		this->m_vao.unbind();
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

		texture::Parameters param{};
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
			texture.setTexParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			texture.setTexParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			texture.setTexParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			texture.setTexParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			this->m_characters.insert_or_assign(c, Character{texture.getId(), ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows), ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top), face->glyph->advance.x});
		}
		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		// create shader program
		textShader.createShaderProgram();
	}

	void TextManager::onDetach() {}

	void TextManager::onRender() {
		textShader.use();
		textShader.setMat4("proj", Application::getTextProjMatrix());
		for (auto t : this->m_text) {
			textShader.setVec3("textColor", t->getColor());
			glActiveTexture(GL_TEXTURE0);
			t->bindVAO();

			auto x = t->getPosition().x;
			auto y = t->getPosition().y;
			auto scale = t->getScale();

			std::string::const_iterator c;
			auto s = t->getText();
			for (c = s.begin(); c != s.end(); ++c) {
				auto ch = this->m_characters[*c];

				f32 xpos = x + ch.bearing.x * scale;
				f32 ypos = y - (ch.size.y - ch.bearing.y) * scale;

				f32 w = ch.size.x * scale;
				f32 h = ch.size.y * scale;

				f32 vertices[6][4] = {
					{xpos, ypos + h, 0.0f, 0.0f},
					{xpos, ypos, 0.0f, 1.0f},
					{xpos + w, ypos, 1.0f, 1.0f},

					{xpos, ypos + h, 0.0f, 0.0f},
					{xpos + w, ypos, 1.0f, 1.0f},
					{xpos + w, ypos + h, 1.0f, 0.0f}};

				glBindTexture(GL_TEXTURE_2D, ch.textureId);
				glBindBuffer(GL_ARRAY_BUFFER, t->getVboId());
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				glDrawArrays(GL_TRIANGLES, 0, 6);

				x += (ch.advance >> 6) * scale;
			}

			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
} // namespace bolt
