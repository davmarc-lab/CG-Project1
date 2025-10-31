#pragma once

#include "../Core/Resource.hpp"
#include "../Graphic.hpp"

#include <iostream>

namespace ogl {
	namespace shader {
		const std::string DEFAULT_SHADER_DIR = "shader/";
		const std::string DEFAULT_SHADER_LOCATION = res::DEFAULT_LOCATION + DEFAULT_SHADER_DIR;

		enum ShaderType {
			SHADER_TYPE_VERTEX,
			SHADER_TYPE_FRAGMENT,
			SHADER_TYPE_GEOMETRY,
			SHADER_TYPE_PROGRAM,
			SHADER_TYPE_NONE,
		};

		inline std::string getShaderTypeString(shader::ShaderType type) {
			switch (type) {
				case SHADER_TYPE_VERTEX:
					return "VERTEX";
				case shader::SHADER_TYPE_FRAGMENT:
					return "FRAGMENT";
				case shader::SHADER_TYPE_GEOMETRY:
					return "GEOMETRY";
				case shader::SHADER_TYPE_PROGRAM:
					return "PROGRAM";
				default:
					return "NONE";
			}
		}

		inline unsigned int getShaderTypeValue(shader::ShaderType type) {
			switch (type) {
				case shader::SHADER_TYPE_VERTEX:
					return GL_VERTEX_SHADER;
				case shader::SHADER_TYPE_FRAGMENT:
					return GL_FRAGMENT_SHADER;
				case shader::SHADER_TYPE_GEOMETRY:
					return GL_GEOMETRY_SHADER;
				case shader::SHADER_TYPE_PROGRAM:
					std::cerr << "Invalid Shader type -> " << getShaderTypeString(type) << ".\nUse a ShaderProgram instead.";
					assert(false);
				default: {
					std::cerr << "Invalid Shader type -> " << getShaderTypeString(type) << ".\n";
					assert(false);
				}
			}
		}
	} // namespace shader

	class Shader {
	public:
		void createShader();

		inline unsigned int getId() const { return this->m_id; }

		Shader() = delete;

		Shader(const std::string &location, const std::string &file, const shader::ShaderType &type) :
			m_location(std::move(location)), m_file(std::move(file)), m_type(type) {}

		Shader(const std::string &file, const shader::ShaderType &type) :
			Shader(shader::DEFAULT_SHADER_LOCATION, file, type) {}

		~Shader() = default;

	private:
		void checkCompileErrors();

		unsigned int m_id = 0;
		shader::ShaderType m_type = shader::ShaderType::SHADER_TYPE_NONE;

		std::string m_location{};
		std::string m_file{};
		unsigned int m_resourceId = 0;
	};

	class ShaderProgram {
	public:
		void createShaderProgram();

		void use() const;
        void clear();

		inline unsigned int getId() const { return this->m_id; }

		void setBool(const std::string &name, bool value) const;
		void setInt(const std::string &name, int value) const;
		void setFloat(const std::string &name, float value) const;

		void setVec2(const std::string &name, const glm::vec2 &value) const;
		void setVec2(const std::string &name, float x, float y) const;

		void setVec3(const std::string &name, const glm::vec3 &value) const;
		void setVec3(const std::string &name, float x, float y, float z) const;

		void setVec4(const std::string &name, const glm::vec4 &value) const;
		void setVec4(const std::string &name, float x, float y, float z, float w) const;

		void setMat2(const std::string &name, const glm::mat2 &mat) const;
		void setMat3(const std::string &name, const glm::mat3 &mat) const;
		void setMat4(const std::string &name, const glm::mat4 &mat) const;

		ShaderProgram() = delete;
		ShaderProgram(const std::string &vloc, const std::string &vfile, const std::string &floc, const std::string &ffile);

		ShaderProgram(const std::string &vfile, const std::string &ffile) :
			ShaderProgram(shader::DEFAULT_SHADER_LOCATION, vfile,
						  shader::DEFAULT_SHADER_LOCATION, ffile) {}

		ShaderProgram(const std::string &vloc, const std::string &vfile, const std::string &floc, const std::string &ffile,
					  const std::string &gloc, const std::string &gfile);

		ShaderProgram(const std::string &vfile, const std::string &ffile, const std::string &gfile) :
			ShaderProgram(shader::DEFAULT_SHADER_LOCATION, vfile,
						  shader::DEFAULT_SHADER_LOCATION, ffile,
						  shader::DEFAULT_SHADER_LOCATION, gfile) {}

		~ShaderProgram();

	private:
		void checkLinkingErrors();

		unsigned int m_id = 0;
		Unique<Shader> m_vert = nullptr;
		Unique<Shader> m_frag = nullptr;
		Unique<Shader> m_geom = nullptr;
	};
} // namespace ogl
