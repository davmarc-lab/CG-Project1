#include "../../include/Core/Resource.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>

namespace ogl {

	void Resource::loadResource() {
		std::ifstream file;
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			file.open(this->m_location + this->m_file.c_str());

			std::stringstream contentStream;
			contentStream << file.rdbuf();
			file.close();

			this->m_content.clear();
			this->m_content = std::move(contentStream.str());
		} catch (std::ifstream::failure e) {
			std::cout << e.what() << "\n";
		}
	}

	void Resource::unloadResource() {
		this->m_content.clear();
		this->m_id = 0;
	}
	unsigned int ResourceManager::addResource(const std::string &file) { return this->addResource(res::DEFAULT_LOCATION, file); }

	unsigned int ResourceManager::addResource(const std::string &location, const std::string &file) {
		this->m_map.insert_or_assign(this->m_currentId, Resource(location, file));
		this->m_map.at(this->m_currentId).loadResource();
		this->m_map.at(this->m_currentId).setResourceId(this->m_currentId);
		this->m_currentId++;
		return this->m_map.rbegin()->first;
		// It seems this works only on linux xD
		// return this->m_map.end()->first;
	}

	bool ResourceManager::removeResource(const unsigned int &id) { return this->m_map.erase(id); }

	Resource ResourceManager::getResource(const unsigned int &id) const {
		try {
			return this->m_map.at(id);
		} catch (const std::out_of_range &oor) {
			std::cerr << "Resource with id = " << id << " not found.\n";
			return res::EMPTY_RESOURCE;
		}
	}

	void ResourceManager::clear() {
		this->m_currentId = 1;
		this->m_map.clear();
	}

} // namespace ogl
