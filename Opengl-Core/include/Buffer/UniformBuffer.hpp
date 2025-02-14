#pragma once

#include "Buffer.hpp"

#include <string>

namespace ogl {
class UniformBuffer : public Buffer {
  public:
    virtual void onAttach();
    virtual void onDetach();

    virtual void bind() const;
    virtual void unbind() const;

    void setup(const unsigned int &size, const unsigned int &bindPoint, const unsigned int &offset, const unsigned int &rangeSize);
    void fastSetup(const unsigned int &size, const unsigned int &bindPoint, const unsigned int &offset, const unsigned int &rangeSize);

    void update(const unsigned int &offset, const unsigned int &size, void *data);
    void fastUpdate(const unsigned int &offset, const unsigned int &size, void *data);

    UniformBuffer() = delete;

    UniformBuffer(const std::string& variable);

    ~UniformBuffer();

  private:
    std::string m_variable{};
    unsigned int m_memAlloc = 0;
};
} // namespace ogl
