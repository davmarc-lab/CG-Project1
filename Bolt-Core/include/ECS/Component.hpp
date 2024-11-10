#pragma once

#include <functional>
#include <vector>
#include <ranges>

#include "../Core/Math.hpp"

#include "../Graphic/Buffer/ElementBuffer.hpp"
#include "../Graphic/Buffer/VertexArray.hpp"
#include "../Graphic/Buffer/VertexBuffer.hpp"

#include "../Graphic/Shader/Shader.hpp"

namespace bolt {
	struct Component {
	public:
		Component() = default;

		virtual ~Component() = default;
	};

	struct Transform : public Component {
	private:
		vec3 position = vec3(0);
		vec3 scale = vec3(1);
		vec3 rotation = vec3(0);
		quat quaternion{};
		mat4 modelMatrix = mat4(1);
		b8 enableModel = true;
		b8 dirty = false;

	public:
		Transform() = default;

		~Transform() override = default;

		inline b8 isModelMatrixEnable() const { return this->enableModel; }

		inline void enableModelMatrix(b8 flag) { this->enableModel = flag; }

		inline const b8& isDirty() const { return this->dirty; }

		inline void setDirty(const b8& dirty) { this->dirty = true; }

		inline const mat4& getModelMatrix() const { return this->modelMatrix; }

		inline void setModelMatrix(mat4 matrix) {
			this->modelMatrix = matrix;
			this->dirty = false;
		}

		inline const vec3& getPosition() const { return this->position; }

		inline void setPosition(vec3 pos) {
			this->position = pos;
			this->dirty = true;
		}

		inline void addPosition(vec3 pos) {
			this->setPosition(this->position += pos);
		}

		inline const vec3& getScale() const { return this->scale; }

		inline void setScale(const vec3& scale) {
			this->scale = scale;
			this->dirty = true;
		}

		inline const vec3& getRotation() const { return this->rotation; }

		inline void setRotation(const vec3& rotation) {
			this->rotation = rotation;
			this->dirty = true;
		}

		inline const quat& getQuaternion() const { return this->quaternion; }

		inline void setQuaternion(quat quaternion) {
			this->quaternion = quaternion;
			this->dirty = true;
		}
	};

	struct Color : public Component {
	public:
		VertexBuffer vbo_c;
		std::vector<vec4> colors{};

		Color() = default;

		~Color() override = default;
	};

	struct RenderComponent : public Component {
	public:
		std::function<void()> draw;

		void setCall(std::function<void()>&& func) { this->draw = std::move(func); }

		void call() { this->draw(); }

		RenderComponent() = default;

		~RenderComponent() override = default;
	};

	struct ShaderComponent : public Component {
	public:
		Unique<ShaderProgram> shader;

		ShaderComponent() = default;

		~ShaderComponent() override = default;
	};

	struct InputComponent : public Component {
	public:
		std::unordered_map<u32, std::function<void()>> callbacks;
		
		inline void registerAction(const u32& key, std::function<void()> &&callback) {
			this->callbacks.emplace(key, std::move(callback));
		}

		inline void call(const u32& key) {
			this->callbacks.at(key)();
		}

		inline std::vector<u32> getAllKeys() const {
			auto kv = std::views::keys(this->callbacks);
			return std::vector<u32>{kv.begin(), kv.end()};
		}
		
		InputComponent() = default;
		
		~InputComponent() override = default;
	};

	struct Mesh : public Component {
	public:
		VertexArray vao;
		VertexBuffer vbo_g;
		VertexBuffer vbo_t;
		ElementBuffer ebo;

		RenderComponent render{};

		std::vector<vec3> vertices{};
		std::vector<u16> indices{};
		Color colorComponent;
		std::vector<vec2> texCoord{};

		b8 instanced = false;

		Mesh() = default;

		~Mesh() override = default;
	};

    enum ColliderType {
        AABB,
        PLANE,
        SPHERE,
        FULL,
    };

    struct Collider : public Component {
    public:
        ColliderType type;

        std::vector<vec3> points{};

        Collider() = default;

        ~Collider() override = default;
    };

	struct PhysicComponent : public Component {
	public:
		f32 mass = 1.f;
		vec3 velocity = vec3(0);
		vec3 acceleration = vec3(0);
		vec3 force = vec3(0);

		PhysicComponent() = default;

		~PhysicComponent() override = default;
	};
} // namespace bolt
