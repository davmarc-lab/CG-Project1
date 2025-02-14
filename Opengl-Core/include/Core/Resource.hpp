#pragma once

#include "Utils.hpp"

#include <map>
#include <string>

namespace ogl {

namespace res {
const std::string DEFAULT_LOCATION = "./resources/";
}

class Resource {
  public:
    inline std::string getResourceLocation() const { return this->m_location.c_str(); }

    inline std::string getResourceName() const { return this->m_name.c_str(); }

    inline std::string getResourceContent() const { return this->m_content.c_str(); }

    inline unsigned int getResourceId() const { return this->m_id; }

    inline void setResourceId(const unsigned int &id) { this->m_id = id; }

    inline bool isResourceLoaded() const { return !this->m_content.empty(); }

    void loadResource();

    void unloadResource();

    Resource() = delete;

    Resource(const std::string &location, const std::string &file) : m_location(std::move(location)), m_file(std::move(file)) {
        this->m_name = std::string(this->m_file);
    }

    Resource(const std::string &file) : Resource(res::DEFAULT_LOCATION, file) {}

    ~Resource() = default;

  private:
    unsigned int m_id = 0;
    std::string m_location;
    std::string m_file;
    std::string m_name;
    std::string m_content;
};

namespace res {
const Resource EMPTY_RESOURCE = Resource("", "");
}

class ResourceManager {
  public:
    unsigned int addResource(const std::string &location, const std::string &file);

    unsigned int addResource(const std::string &file);

    bool removeResource(const unsigned int& id);

    inline unsigned int getCurrentId() const { return this->m_currentId; }

    Resource getResource(const unsigned int &id) const;

    ResourceManager(ResourceManager &other) = delete;

    void operator=(const ResourceManager &other) = delete;

    inline static Shared<ResourceManager> instance() {
        if (s_pointer == nullptr) {
            Shared<ResourceManager> copy(new ResourceManager());
            copy.swap(s_pointer);
        }
        return s_pointer;
    }

  private:
    inline static Shared<ResourceManager> s_pointer = nullptr;
    unsigned int m_currentId = 1;

    std::map<unsigned int, Resource> m_map{};

    ResourceManager() = default;
};

} // namespace ogl
