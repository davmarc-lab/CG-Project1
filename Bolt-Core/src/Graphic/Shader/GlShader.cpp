#include "../../../include/Graphic/Shader/Shader.hpp"

#include "../../../../Bolt-Graphics/include/glad/glad.h"

#include <iostream>
#include <cassert>

namespace bolt {
	GLenum getShaderTypeValue(shader::ShaderType type) {
		switch (type) {
			case shader::SHADER_VERTEX: return GL_VERTEX_SHADER;
			case shader::SHADER_FRAGMENT: return GL_FRAGMENT_SHADER;
			case shader::SHADER_GEOMETRY: return GL_GEOMETRY_SHADER;
			case shader::SHADER_PROGRAM: /* BT_ERROR_CORE("Cannot find the correct shader type."); */ assert(false);
			default: /* BT_ERROR_CORE("Cannot find the correct shader type."); */ assert(false);
		}
	}

	std::string getShaderTypeString(shader::ShaderType type) {
		switch (type) {
			case shader::SHADER_VERTEX: return "VERTEX";
			case shader::SHADER_FRAGMENT: return "FRAGMENT";
			case shader::SHADER_GEOMETRY: return "GEOMETRY";
			case shader::SHADER_PROGRAM: return "PROGRAM";
			default: return "NONE";
		}
	}

	void checkCompileErrors(u32 shader, shader::ShaderType type) {
		std::string strType = getShaderTypeString(type);
		i32 success;
		i8 log[1024];

		if (type != shader::ShaderType::SHADER_PROGRAM) {
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 1024, NULL, log);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << strType << "\n"
					<< log
					<< "\n-- -------------------------------------------------- --\n";
			}
		}
	}

	void checkLinkingError(u32 id) {
		i32 success;
		i8 log[1024];

		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(id, 1024, NULL, log);
			std::cout << "ERROR::SHADER_PROGRAM_LINKING_ERROR of type: " << "PROGRAM" << "\n"
				<< log
				<< "\n-- -------------------------------------------------- --\n";
		}
	}

	void Shader::createShader() {
		this->m_resource->loadResource();
		assert(this->isShaderCodeLoaded());

		assert(this->m_type != shader::ShaderType::SHADER_NONE);
		assert(this->m_type != shader::ShaderType::SHADER_PROGRAM);

		auto shaderType = getShaderTypeValue(this->m_type);
		GLuint shaderId = glCreateShader(shaderType);

		std::string strCode = this->m_resource->getResourceContent();
		i32 lenght = static_cast<i32>(strCode.length());
		const char* code = strCode.c_str();

		glShaderSource(shaderId, 1, &code, &lenght);
		glCompileShader(shaderId);
		checkCompileErrors(shaderId, this->m_type);
		this->setShaderId(shaderId);
	}

	Shader::~Shader() {
		this->m_resource->unloadResource();
	}

	void ShaderProgram::createShaderProgram() {
		if (!this->m_vert->isShaderCreated())
			this->m_vert->createShader();

		if (!this->m_frag->isShaderCreated())
			this->m_frag->createShader();

		if (this->m_geom != nullptr) {
			if (!this->m_geom->isShaderCreated())
				this->m_geom->createShader();
		}

		this->m_id = glCreateProgram();
		glAttachShader(this->m_id, this->m_vert->getShaderId());
		glAttachShader(this->m_id, this->m_frag->getShaderId());
		if (this->m_geom != nullptr)
			glAttachShader(this->m_id, this->m_geom->getShaderId());
		glLinkProgram(this->m_id);
		checkLinkingError(this->m_id);

		glDeleteShader(this->m_vert->getShaderId());
		glDeleteShader(this->m_frag->getShaderId());
	}

	void ShaderProgram::use() const {
		glUseProgram(this->m_id);
	}

	void ShaderProgram::setBool(const std::string& name, b8 value) {
		glUniform1i(glGetUniformLocation(this->getId(), name.c_str()), (int)value);
	}

	void ShaderProgram::setInt(const std::string& name, i32 value) {
		glUniform1i(glGetUniformLocation(this->getId(), name.c_str()), value);
	}

	void ShaderProgram::setFloat(const std::string& name, f32 value) {
		glUniform1f(glGetUniformLocation(this->getId(), name.c_str()), value);
	}

	void ShaderProgram::setVec2(const std::string& name, const vec2& value) {
		glUniform2fv(glGetUniformLocation(this->getId(), name.c_str()), 1, &value[0]);
	}

	void ShaderProgram::setVec2(const std::string& name, f32 x, f32 y) {
		glUniform2f(glGetUniformLocation(this->getId(), name.c_str()), x, y);
	}

	void ShaderProgram::setVec3(const std::string& name, const vec3& value) {
		glUniform3fv(glGetUniformLocation(this->getId(), name.c_str()), 1, &value[0]);
	}

	void ShaderProgram::setVec3(const std::string& name, f32 x, f32 y, f32 z) {
		glUniform3f(glGetUniformLocation(this->getId(), name.c_str()), x, y, z);
	}

	void ShaderProgram::setVec4(const std::string& name, const vec4& value) {
		glUniform4fv(glGetUniformLocation(this->getId(), name.c_str()), 1, &value[0]);
	}

	void ShaderProgram::setVec4(const std::string& name, f32 x, f32 y, f32 z, f32 w) {
		glUniform4f(glGetUniformLocation(this->getId(), name.c_str()), x, y, z, w);
	}

	void ShaderProgram::setMat2(const std::string& name, const mat2& mat) {
		glUniformMatrix2fv(glGetUniformLocation(this->getId(), name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void ShaderProgram::setMat3(const std::string& name, const mat3& mat) {
		glUniformMatrix3fv(glGetUniformLocation(this->getId(), name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void ShaderProgram::setMat4(const std::string& name, const mat4& mat) {
		glUniformMatrix4fv(glGetUniformLocation(this->getId(), name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
} // namespace Bolt
