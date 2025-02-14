#pragma once

#include "Buffer.hpp"
#include <cstddef>

namespace ogl {
class ElementBuffer : public Buffer {
  public:
    template <typename T> inline void setup(const T *indices, const size_t &size, const unsigned int &usage) const {
        this->bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(T) * size, indices, usage);
    }

    virtual void onAttach() override;
    virtual void onDetach() override;

    virtual void bind() const override;
    virtual void unbind() const override;

    ElementBuffer() = default;
    ~ElementBuffer() override;
};
} // namespace ogl
