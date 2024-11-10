#include "../../include/Core/Resource.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>

namespace bolt {
	void Resource::loadResource() {
		std::ifstream file;
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			file.open(this->m_location->append(this->m_name->c_str()).c_str());

			std::stringstream contentStream;
			contentStream << file.rdbuf();
			file.close();

			this->m_content.clear();
			this->m_content = contentStream.str();
		} catch (std::ifstream::failure e) {
			// BT_ERROR_CORE("Failed to load resource: name -> {0}\n\tError -> {1}", this->m_name, e.what());
			std::cout << e.what() << "\n";
		}
	}

	void Resource::unloadResource() {
		// I shouldn't call destructor
		// this->~Resource();
	}
} // namespace Bolt
