#pragma once

#include <mutex>
#include <string>
#include <utility>

#include "Utils.hpp"

namespace bolt {
	namespace res {
		BT_AUTOCONST DEFAULT_LOCATION = std::string("./resources/");
	}

	class Resource {
	private:
		u32 m_id = 0;
		Unique<std::string> m_location;
		Unique<std::string> m_name;
		std::string m_content;

	public:
		Resource() : Resource(res::DEFAULT_LOCATION, "") {}

		Resource(std::string location, std::string name) : m_location(CreateUnique<std::string>(std::move(location))), m_name(CreateUnique<std::string>(std::move(name))) {}

		Resource(std::string name) : Resource(res::DEFAULT_LOCATION, name) {}

		~Resource() = default;

		inline std::string getResourceLocation() const { return this->m_location->c_str(); }

		inline std::string getResourceName() const { return this->m_name->c_str(); }

		inline std::string getResourceContent() const { return this->m_content; }

		inline u32 getResourceId() const { return this->m_id; }

		inline void setResourceId(u32 id) { this->m_id = id; }

		inline b8 isResourceLoaded() const { return !this->m_content.empty(); }

		void loadResource();

		void unloadResource();
	};

	class ResourceManager {
	private:
		inline static Shared<ResourceManager> s_pointer = nullptr;
		inline static std::mutex s_mutex;

		u32 m_currentId = 0;

		ResourceManager() = default;

	public:
		ResourceManager(ResourceManager& other) = delete;

		void operator=(const ResourceManager& other) = delete;

		void loadResource();

		void unloadResource();

		/*
		 * Retrieves the instance of the `ResourceManager` if it's not created.
		 * This function is thread safe using a simple `std::mutex`.
		 *
		 * @return `ResourceManager` shared pointer.
		 */
		inline static Shared<ResourceManager> instance() {
			std::lock_guard<std::mutex> lock(s_mutex);
			if (s_pointer == nullptr) {
				Shared<ResourceManager> copy(new ResourceManager());
				copy.swap(s_pointer);
			}

			return s_pointer;
		}
	};
} // namespace Bolt
