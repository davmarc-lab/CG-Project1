#pragma once

#include "../../Core/Resource.hpp"
#include "../../Core/Utils.hpp"
#include "../../Core/Math.hpp"

#include <string>

namespace bolt {
	namespace shader {
		enum ShaderType {
			SHADER_VERTEX,
			SHADER_FRAGMENT,
			SHADER_GEOMETRY,
			SHADER_PROGRAM,
			SHADER_NONE,
		};
	}

	class Shader {
	protected:
		shader::ShaderType m_type = shader::SHADER_NONE;
		Unique<Resource> m_resource;
		u32 m_id = 0;

	public:
		virtual void createShader();

		inline u32 getShaderId() const { return this->m_id; }

		inline void setShaderId(u32 id) { this->m_id = id; }

		inline shader::ShaderType getShaderType() const { return this->m_type; }

		inline std::string getShaderPath() const { return this->m_resource->getResourceLocation().append(this->m_resource->getResourceName()).c_str(); }

		inline b8 isShaderCodeLoaded() const { return this->m_resource->isResourceLoaded(); }

		inline b8 isShaderCreated() const { return this->m_id != 0; }

		Shader() = delete;

		Shader(std::string location, std::string name, shader::ShaderType type) : m_type(type) { this->m_resource = CreateUnique<Resource>(location, name); }

		Shader(std::string name, shader::ShaderType type) : Shader(res::DEFAULT_LOCATION, name, type) {}

		virtual ~Shader();
	};

	class ShaderProgram {
	public:
		void use() const;

		inline u32 getId() const { return this->m_id; }

		void createShaderProgram();

		inline b8 isProgramCreated() const { return this->m_id != 0; }

		void setBool(const std::string& name, b8 value);
		void setInt(const std::string& name, i32 value);
		void setFloat(const std::string& name, f32 value);

		void setVec2(const std::string& name, const vec2& value);
		void setVec2(const std::string& name, f32 x, f32 y);

		void setVec3(const std::string& name, const vec3& value);
		void setVec3(const std::string& name, f32 x, f32 y, f32 z);

		void setVec4(const std::string& name, const vec4& value);
		void setVec4(const std::string& name, f32 x, f32 y, f32 z, f32 w);

		void setMat2(const std::string& name, const mat2& mat);
		void setMat3(const std::string& name, const mat3& mat);
		void setMat4(const std::string& name, const mat4& mat);

		ShaderProgram() = default;

		ShaderProgram(Unique<Shader> vert, Unique<Shader> frag) : m_vert(std::move(vert)), m_frag(std::move(frag)) {}

		ShaderProgram(const std::string& vert, const std::string& frag, const std::string& geom = "") {
			this->m_vert = CreateUnique<Shader>(vert, shader::SHADER_VERTEX);
			this->m_frag = CreateUnique<Shader>(frag, shader::SHADER_FRAGMENT);
			if (!geom.empty())
				this->m_geom = CreateUnique<Shader>(geom, shader::SHADER_GEOMETRY);
		}

		~ShaderProgram() = default;

	private:
		u32 m_id = 0;
		Unique<Shader> m_vert = nullptr;
		Unique<Shader> m_frag = nullptr;
		Unique<Shader> m_geom = nullptr;
	};
} // namespace bolt
