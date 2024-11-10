#pragma once

#include <string>
#include "Core.hpp"
#include "Utils.hpp"

#define GLM_ENABLE_EXPERIMENTAL

// glm library has a problem with absolute patterns
#include "../../../Bolt-Vendor/include/glm/gtc/quaternion.hpp"
#include "../../../Bolt-Vendor/include/glm/gtc/type_ptr.hpp"
#include "../../../Bolt-Vendor/include/glm/gtx/quaternion.hpp"
#include "../../../Bolt-Vendor/include/glm/gtx/string_cast.hpp"

namespace bolt {
	using glm::vec2;
	using glm::vec3;
	using glm::vec4;

	using glm::ivec2;
	using glm::ivec3;
	using glm::ivec4;

	using glm::mat2;
	using glm::mat3;
	using glm::mat4;

	using glm::quat;

	using glm::ortho;
	using glm::perspective;

	using glm::lookAt;
	using glm::quatLookAt;

	using glm::rotate;
	using glm::scale;
	using glm::toMat4;
	using glm::translate;

	using glm::degrees;
	using glm::radians;

	using glm::abs;
	using glm::clamp;
	using glm::lerp;

	using glm::acos;
	using glm::asin;
	using glm::atan;
	using glm::cos;
	using glm::pi;
	using glm::sin;
	using glm::tan;

	using glm::cross;
	using glm::dot;
	using glm::inverse;
	using glm::normalize;

	using glm::to_string;
	using glm::value_ptr;

	BT_CONSTEXPR auto identity = mat4(1);
	BT_CONSTEXPR auto FPI = glm::pi<f32>();
	BT_CONSTEXPR auto DPI = glm::pi<f64>();
} // namespace bolt

namespace bmath {
	BT_CONSTEXPR f32 F_EPSILON = 1e-9f;

	inline b8 fequal(const f32& a, const f32& b) {
		return std::abs(a - b) < F_EPSILON;
	}

	struct bvec2 {
		f32 x, y;

		bvec2() : bvec2(0) {}

		bvec2(const f32& x, const f32& y) : x(x), y(y) {}

		bvec2(const f32& x) : bvec2(x, x) {}

		bvec2(const bvec2& vec) : bvec2(vec.x, vec.y) {}

		f32& operator[](std::size_t index) {
			BT_ASSERT(index < 2);
			switch (index) {
				default:
				case 0: return this->x;
				case 1: return this->y;
			}
		}

		friend bvec2 operator+(const bvec2& a, const bvec2& b) {
			return {a.x + b.x, a.y + b.y};
		}

		friend bvec2 operator-(const bvec2& a, const bvec2& b) {
			return {a.x - b.x, a.y - b.y};
		}

		friend bvec2 operator*(const bvec2& a, const bvec2& b) {
			return {a.x * b.x, a.y * b.y};
		}

		friend bvec2 operator/(const bvec2& a, const bvec2& b) {
			return {a.x / b.x, a.y / b.y};
		}

		friend b8 operator==(const bvec2& a, const bvec2& b) {
			return a.x == b.x && a.y == b.y;
		}

		inline f32 dot(const bvec2& vec) {
			return this->x * vec.x + this->y * vec.y;
		}
	};

	inline std::string dump(const bvec2& vec) {
		return std::string("Vec2: x = ").append(std::to_string(vec.x)).append(", y = ").append(std::to_string(vec.y));
	}

	struct bvec3 {
		f32 x, y, z;

		bvec3() : bvec3(0) {}

		bvec3(const f32& x, const f32& y, const f32& z) : x(x), y(y), z(z) {}

		bvec3(const f32& x) : bvec3(x, x, x) {}

		bvec3(const i32& x) : bvec3(x, x, x) {}

		bvec3(const bvec2& vec, const f32& z) : bvec3(vec.x, vec.y, z) {}

		bvec3(const bvec3& vec) : bvec3(vec.x, vec.y, vec.z) {}

		f32& operator[](std::size_t index) {
			BT_ASSERT(index < 3);
			switch (index) {
				default:
				case 0: return this->x;
				case 1: return this->y;
				case 2: return this->z;
			}
		}

		friend bvec3 operator+(const bvec3& a, const bvec3& b) {
			return {a.x + b.x, a.y + b.y, a.z + b.z};
		}

		friend bvec3 operator-(const bvec3& a, const bvec3& b) {
			return {a.x - b.x, a.y - b.y, a.z - b.z};
		}

		friend bvec3 operator*(const bvec3& a, const bvec3& b) {
			return {a.x * b.x, a.y * b.y, a.z * b.z};
		}

		friend bvec3 operator/(const bvec3& a, const bvec3& b) {
			return {a.x / b.x, a.y / b.y, a.z / b.z};
		}

		friend b8 operator==(const bvec3& a, const bvec3& b) {
			return a.x == b.x && a.y == b.y && a.z == b.z;
		}

		inline f32 dot(const bvec3& vec) {
			return this->x * vec.x + this->y * vec.y + this->z * vec.z;
		}
	};

	inline std::string dump(const bvec3& vec) {
		return std::string("Vec3: x = ").append(std::to_string(vec.x)).append(", y = ").append(std::to_string(vec.y)).append(", z = ").append(std::to_string(vec.z));
	}

	struct bvec4 {
		f32 r = 0, g = 0, b = 0, a = 0;

		bvec4(const f32& r = 0, const f32& g = 0, const f32& b = 0, const f32& a = 0) : r(r), g(g), b(b), a(a) {}

		bvec4(const f32& x) : bvec4(x, x, x, x) {}

		bvec4(const i32& x) : bvec4(x, x, x, x) {}

		bvec4(const bvec3& vec, const f32& a) : bvec4(vec.x, vec.y, vec.z, a) {}

		bvec4(const bvec4& vec) : bvec4(vec.r, vec.g, vec.b, vec.a) {}

		f32& operator[](std::size_t index) {
			BT_ASSERT(index < 4);
			switch (index) {
				default:
				case 0: return this->r;
				case 1: return this->g;
				case 2: return this->b;
				case 3: return this->a;
			}
		}

		friend bvec4 operator+(const bvec4& a, const bvec4& b) {
			return {a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a};
		}

		friend bvec4 operator-(const bvec4& a, const bvec4& b) {
			return {a.r - b.r, a.g - b.g, a.b - b.b, a.a - b.a};
		}

		friend bvec4 operator*(const bvec4& a, const bvec4& b) {
			return {a.r * b.r, a.g * b.g, a.b * b.b, a.a * b.a};
		}

		friend bvec4 operator/(const bvec4& a, const bvec4& b) {
			return {a.r / b.r, a.g / b.g, a.b / b.b, a.a / b.a};
		}

		friend b8 operator==(const bvec4& a, const bvec4& b) {
			return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
		}

		inline f32 dot(const bvec4& vec) {
			return this->r * vec.r + this->g * vec.g + this->b * vec.b + this->a * vec.a;
		}
	};

	inline std::string dump(const bvec4& vec) {
		return std::string("Vec4: r = ").append(std::to_string(vec.r)).append(", g = ").append(std::to_string(vec.g)).append(", b = ").append(std::to_string(vec.b)).append(", a = ").append(std::to_string(vec.a));
	}

	struct bmat2 {
		bvec2 value[2];

		bmat2() : bmat2(bvec2(0), bvec2(0)) {}

		bmat2(const f32& x, const f32& y) : bmat2(bvec2(x, y)) {}

		bmat2(const bvec2& first) : bmat2(first, first) {}

		bmat2(const bvec2& first, const bvec2& second) {
			this->value[0] = first;
			this->value[1] = second;
		}

		bmat2(const bmat2& mat) : bmat2(mat.value[0], mat.value[0]) {}

		bmat2(const f32& val) : bmat2({val}, {val}) {}

		bvec2& operator[](std::size_t index) {
			BT_ASSERT(index < 2);
			return this->value[index];
		}

		friend bmat2 operator+(const bmat2& a, const bmat2& b) {
			return {a.value[0] + b.value[0], a.value[1] + b.value[1]};
		}

		friend bmat2 operator-(const bmat2& a, const bmat2& b) {
			return {a.value[0] - b.value[0], a.value[1] - b.value[1]};
		}

		friend bmat2 operator*(bmat2& a, bmat2& b) {
			return bmat2(
				{
					a[0][0] * b[0][0] + a[0][1] * b[1][0],
					a[0][0] * b[0][1] + a[0][1] * b[1][1],
				},
				{
					a[1][0] * b[0][0] + a[1][1] * b[1][0],
					a[1][0] * b[0][1] + a[1][1] * b[1][1],
				}
			);
		}

		bmat2 operator*(const f32& val) {
			return {this->value[0] * val, this->value[1] * val};
		}

		friend bmat2 operator/(const bmat2& a, const bmat2& b) {
			return {a.value[0] / b.value[0], a.value[1] / b.value[1]};
		}

		bmat2 operator/(const f32& val) {
			return {this->value[0] / val, this->value[1] / val};
		}

		inline bmat2 transpose() {
			return {{this->value[0].x, this->value[1].x}, {this->value[0].y, this->value[1].y}};
		}
	};

	inline std::string dump(const bmat2& mat) {
		return std::string("Mat2:\n").append(dump(mat.value[0])).append("\n").append(dump(mat.value[1]));
	}

	struct bmat3 {
		bvec3 first, second, third;
	};
} // namespace bmath
