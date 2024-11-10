#pragma once

#include <string>

namespace bolt {
	class Entity final {
	private:
		std::string m_name;

	public:
		explicit Entity(std::string name = "Entity") : m_name(std::move(name)) {}

		~Entity() = default;

		std::string getName() const { return this->m_name; }

		void setName(const std::string& name) { this->m_name = name; }
	};
} // namespace Bolt
